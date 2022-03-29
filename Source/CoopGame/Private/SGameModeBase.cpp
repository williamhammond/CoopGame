#include "SGameModeBase.h"

#include "SGameStateBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"



bool ASGameModeBase::IsMissionSuccess_Implementation()
{
	return false;
}

bool ASGameModeBase::IsMissionFailure_Implementation()
{
	return false; 
}


ASGameModeBase::ASGameModeBase()
{
	GameStateClass = ASGameStateBase::StaticClass();
}

void ASGameModeBase::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (SpectatingViewpointClass)
	{
		TArray<AActor*> ReturnedActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

		if (ReturnedActors.Num() > 0)
		{
			AActor* NewViewTarget = ReturnedActors[0];
			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
			{
				APlayerController* PlayerController = It->Get();
				PlayerController->SetViewTargetWithBlend(NewViewTarget, 0.5f,
				                                         VTBlend_Cubic);
			}
		}
	}

	ASGameStateBase* GameState = GetGameState<ASGameStateBase>();
	if (GameState)
	{
		GameState->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}

	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}

void ASGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsMissionFailure())
	{
		CompleteMission(nullptr, false);
	}
	if (IsMissionSuccess())
	{
		CompleteMission(nullptr, true);
	}
}
