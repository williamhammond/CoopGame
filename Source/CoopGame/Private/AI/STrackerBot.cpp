#include "AI/STrackerBot.h"

#include "DrawDebugHelpers.h"
#include "SHealthComponent.h"
#include "AI/Navigation/NavigationPath.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceDynamic.h"


ASTrackerBot::ASTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::OnHealthChanged);

	bUseVelocityChange = true;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta,
                                   const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (MaterialInstance)
	{
		MaterialInstance->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	} else {
		MaterialInstance = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
	}

	
	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());
}

FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystem::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	return GetActorLocation();
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float const DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
	if (DistanceToTarget <= RequiredDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
		DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		ForceDirection *= MovementForce;
		MeshComponent->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32,
		                          FColor::Yellow, false, 0.0f, 0, 1.0f);
	}
	DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Yellow, false, 0.0f, 1.0f);
}
