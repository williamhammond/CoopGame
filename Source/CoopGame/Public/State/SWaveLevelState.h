#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SWaveLevelState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,
	WaveInProgress,
	WaitingToComplete,
	WaveComplete,
	GameOver,
};

UCLASS()
class COOPGAME_API ASWaveLevelState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category="GameState")
	void WaveStateChange(EWaveState NewState, EWaveState OldState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveState, Category="GameState")
	EWaveState WaveState;

public:
	void SetWaveState(EWaveState NewState);
};
