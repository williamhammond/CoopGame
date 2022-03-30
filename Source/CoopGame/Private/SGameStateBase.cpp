#include "SGameStateBase.h"

#include "SCharacter.h"
#include "SPlayerController.h"
#include "Engine/World.h"


void ASGameStateBase::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ASPlayerController* PlayerController = Cast<ASPlayerController>(It->Get());
		if (PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			APawn* Pawn = PlayerController->GetPawn();
			if (Pawn)
			{
				Pawn->DisableInput(nullptr);
			}
			ASCharacter* character = Cast<ASCharacter>(Pawn);
			if (character)
			{
				character->StopFire();
			}
		}
	}
}
