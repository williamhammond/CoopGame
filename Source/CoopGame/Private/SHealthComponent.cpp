#include "SHealthComponent.h"


USHealthComponent::USHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Health = 100;
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}
