#include "AI/STrackerBot.h"

#include "Components/StaticMeshComponent.h"


ASTrackerBot::ASTrackerBot()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent-> SetCanEverAffectNavigation(false);
	RootComponent = MeshComponent;
	PrimaryActorTick.bCanEverTick = true;
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}