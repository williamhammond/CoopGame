#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

UCLASS()
class COOPGAME_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bIsMissionSuccess);
};
