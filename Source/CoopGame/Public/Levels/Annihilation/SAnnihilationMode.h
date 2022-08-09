#pragma once

#include "CoreMinimal.h"
#include "SGameModeBase.h"
#include "SAnnihilationMode.generated.h"

UCLASS()
class COOPGAME_API ASAnnihilationMode : public ASGameModeBase
{
	GENERATED_BODY()

protected:
	virtual bool IsMissionSuccess_Implementation() override;
	virtual bool IsMissionFailure_Implementation() override;

	bool AnyPlayerAlive();
	bool AnyVillainAlive() const;
public:
	ASAnnihilationMode();
};
