#include "Annihilation/SAnnihilationMode.h"

#include "SCharacterHUD.h"
#include "Engine/World.h"
#include "SHealthComponent.h"

ASAnnihilationMode::ASAnnihilationMode ()
{
	HUDClass = ASCharacterHUD::StaticClass();	
	PrimaryActorTick.bCanEverTick = true;
}

bool ASAnnihilationMode::IsMissionSuccess_Implementation()
{
	return !AnyVillainAlive();
}

bool ASAnnihilationMode::IsMissionFailure_Implementation()
{
	return !AnyPlayerAlive();
}

bool ASAnnihilationMode::AnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		APawn* MyPawn = PlayerController->GetPawn();
		if (PlayerController && MyPawn)
		{
			USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComponent) && HealthComponent->GetHealth() > 0.0f)
			{
				return true;
			}
		}
	}
	return false;
}

bool ASAnnihilationMode::AnyVillainAlive()
{
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* Pawn = It->Get();

		if (Pawn &&
			Pawn->Tags.Contains("villain"))
		{
			USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				Pawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComponent) && HealthComponent->GetHealth() > 0.0f)
			{
				return true;
			}
		}
	}
	return false;
}
