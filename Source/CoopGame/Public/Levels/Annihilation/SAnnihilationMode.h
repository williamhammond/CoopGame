#pragma once

#include "CoreMinimal.h"
#include "SGameModeBase.h"
#include "SAnnihilationMode.generated.h"

UCLASS()
class COOPGAME_API ASAnnihilationMode : public ASGameModeBase 
{
	GENERATED_BODY()

protected:
	bool IsMissionSuccess_Implementation() override;
	bool IsMissionFailure_Implementation() override;

	bool AnyPlayerAlive();
	bool AnyVillianAlive();
public:
	ASAnnihilationMode();
};
