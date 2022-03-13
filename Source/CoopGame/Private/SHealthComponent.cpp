#include "SHealthComponent.h"

#include "SGameMode.h"
#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100;
	bIsDead = false;

	TeamNum = 255;

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
	if ((Damage <= 0.0f || bIsDead || IsFriendly(DamagedActor, DamageCauser)) && DamagedActor != DamageCauser)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
	bIsDead = Health <= 0;
	if (bIsDead)
	{
		ASGameMode* GameMode = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float const Damage = Health - OldHealth;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

float USHealthComponent::GetHealth() const
{
	return Health;
}

void USHealthComponent::Heal(float Amount)
{
	if (Amount <= 0 || Health <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health + Amount, 0.0f, DefaultHealth);
	OnHealthChanged.Broadcast(this, Health, Amount, nullptr, nullptr, nullptr);
}

bool USHealthComponent::IsFriendly(AActor* A, AActor* B)
{
	if (!A || !B)
	{
		return true;
	}

	USHealthComponent* HealthComponentA = Cast<USHealthComponent>(
		A->GetComponentByClass(USHealthComponent::StaticClass()));

	USHealthComponent* HealthComponentB = Cast<USHealthComponent>(
		B->GetComponentByClass(USHealthComponent::StaticClass()));

	if (!HealthComponentA || !HealthComponentB)
	{
		return true;
	}

	return HealthComponentA->TeamNum == HealthComponentB->TeamNum;
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(USHealthComponent, Health);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
