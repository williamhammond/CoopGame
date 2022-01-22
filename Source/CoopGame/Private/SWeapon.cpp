// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"

#include "CoopGame.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
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

// Sets default values
ASWeapon::ASWeapon()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";

	BaseDamage = 20.0f;

	RateOfFire = 600.0f;
	SetReplicates(true);
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60 / RateOfFire;
}


void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		return;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();

	FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TracerEndpoint = TraceEnd;

	FHitResult Hit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);
	if (isHit)
	{
		TracerEndpoint = Hit.Location;
		AActor* HitActor = Hit.GetActor();
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		float EffectiveDamage = BaseDamage;
		if (SurfaceType == SURFACE_FLESHVULNERABLE)
		{
			EffectiveDamage *= 4.0f;
		}
		UGameplayStatics::ApplyPointDamage(HitActor, EffectiveDamage, ShotDirection, Hit,
		                                   MyOwner->GetInstigatorController(),
		                                   this, DamageType);


		UParticleSystem* SelectedEffect = nullptr;
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
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint,
			                                         Hit.ImpactNormal.Rotation());
		}
	}
	if (DebugWeaponDrawing > 0)
	{
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
	PlayFireEffects(TracerEndpoint);
	LastFireTime = GetWorld()->TimeSeconds;
}

void ASWeapon::StartFire()
{
	const float FirstDelay = FMath::Max(0.0f, LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetween_Shots, this, &ASWeapon::Fire, TimeBetweenShots, true,
	                                FirstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetween_Shots);
}

void ASWeapon::PlayFireEffects(FVector TracerEndpoint)
{
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

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PlayerController = Cast<APlayerController>(MyOwner->GetController());
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(FireCameraShake);
		}
	}
}
