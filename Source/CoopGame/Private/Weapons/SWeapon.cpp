
#include "Weapons/SWeapon.h"

#include "CoopGame.h"
#include "DrawDebugHelpers.h"
#include "SPlayerController.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVarDebugWeaponDrawing(TEXT("COOP.DebugWeapons"),
                                               DebugWeaponDrawing,
                                               TEXT("Draw Debug Lines for Weapons"),
                                               ECVF_Cheat);

ASWeapon::ASWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";

	BaseDamage = 20.0f;

	RateOfFire = 600.0f;
	BulletSpreadDegrees = 2.0f;

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;

	SetReplicates(true);
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateOfFire;
}


void ASWeapon::Fire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerFire();
	}

	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		return;
	}
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();

	float HalfRad = FMath::DegreesToRadians(BulletSpreadDegrees);
	ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

	FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TracerEndpoint = TraceEnd;

	EPhysicalSurface SurfaceType = SurfaceType_Default;
	FHitResult Hit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);
	if (isHit)
	{
		TracerEndpoint = Hit.Location;
		AActor* HitActor = Hit.GetActor();
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		float EffectiveDamage = BaseDamage;
		if (SurfaceType == SURFACE_FLESHVULNERABLE)
		{
			EffectiveDamage *= 4.0f;
		}

		UGameplayStatics::ApplyPointDamage(HitActor, EffectiveDamage, ShotDirection, Hit,
		                                   MyOwner->GetInstigatorController(),
		                                   MyOwner, DamageType);
		PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
		HitScanTrace.SurfaceType = SurfaceType;
	}

	if (DebugWeaponDrawing > 0)
	{
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
	if (GetLocalRole() == ROLE_Authority)
	{
		HitScanTrace.TraceTo = TracerEndpoint;
	}
	PlayFireEffects(TracerEndpoint);
	LastFireTime = GetWorld()->TimeSeconds;
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::OnRep_HitScanTrace()
{
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(0.0f, LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetween_Shots, this, &ASWeapon::Fire, TimeBetweenShots, true,
	                                FirstDelay);
}

void ASWeapon::StopFire()
{
	const APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		ASPlayerController* PlayerController = Cast<ASPlayerController>(MyOwner->GetController());
		if (PlayerController)
		{
			PlayerController->ClientStopCameraShake(FireCameraShake);
		}
	}
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetween_Shots);
}

void ASWeapon::PlayFireEffects(FVector TracerEndpoint)
{
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		const FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerTargetName, TracerEndpoint);
		}
	}

	const APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		ASPlayerController* PlayerController = Cast<ASPlayerController>(MyOwner->GetController());
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(FireCameraShake);
		}
	}
}

void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint) const
{
	UParticleSystem* SelectedEffect;
	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEvent;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		const FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint,
		                                         ShotDirection.Rotation());
	}
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
