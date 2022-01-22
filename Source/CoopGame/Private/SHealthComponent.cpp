#include "SHealthComponent.h"

#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"


USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100;
	SetIsReplicated(true);
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeDamage);
		}
	}
	Health = DefaultHealth;
}

void USHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                         AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(USHealthComponent, Health);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
