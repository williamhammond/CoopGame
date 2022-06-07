#include "AI/STrackerBot.h"

#include "DrawDebugHelpers.h"
#include "ExceptionHandling.h"
#include "SCharacter.h"
#include "SHealthComponent.h"
#include "STrackerBotController.h"
#include "TimerManager.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

static int32 DebugTrackerBotDrawing = 0;
FAutoConsoleVariableRef CVarDebugTrackerBotDrawing(TEXT("COOP.DebugTrackerBot"),
                                                   DebugTrackerBotDrawing,
                                                   TEXT("Draw Debug Lines for Tracker Bots"),
                                                   ECVF_Cheat);

ASTrackerBot::ASTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::OnHealthChanged);

	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetSphereRadius(200);
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComponent->SetupAttachment(RootComponent);

	Target = nullptr;

	bUseVelocityChange = true;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;
	ExplosionDamage = 60;
	ExplosionRadius = 350;
	SelfDamageInterval = 0.25f;
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		RefreshPath();
	}
}

void ASTrackerBot::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
                                   const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (MaterialInstance)
	{
		MaterialInstance->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}
	else
	{
		MaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(
			0, MeshComponent->GetMaterial(0));
	}

	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	if (Target)
	{
		UNavigationPath* NavPath =
			UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), Target);


		GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
		GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ASTrackerBot::RefreshPath, 5.0f, false);
		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	MeshComponent->SetVisibility(false, true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (GetLocalRole() == ROLE_Authority)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr,
		                                    IgnoredActors, this,
		                                    GetInstigatorController(), true);
		if (DebugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		}
		SetLifeSpan(2.0f);
	}
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
	if (DebugTrackerBotDrawing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tracker bot health is %f"), HealthComponent->GetHealth());
	}
}

void ASTrackerBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::Move()
{
	if (GetLocalRole() == ROLE_Authority && !bExploded)
	{
		const float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
			if (DebugTrackerBotDrawing)
			{
				DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
			}
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();

			ForceDirection *= MovementForce;
			MeshComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
			if (DebugTrackerBotDrawing)
			{
				DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32,
				                          FColor::Yellow, false, 0.0f, 0, 1.0f);
			}
		}
		if (DebugTrackerBotDrawing)
		{
			DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
		}
	}
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const ASTrackerBotController* PlayerController = Cast<ASTrackerBotController>(GetController());
	Target = PlayerController->GetTarget();
	Move();
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (bStartedSelfDestruction || bExploded)
	{
		return;
	}
	const ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
	if (PlayerPawn && !USHealthComponent::IsFriendly(OtherActor, this))
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval,
			                                true, 0.0f);
		}
		bStartedSelfDestruction = true;
		UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
	}
}
