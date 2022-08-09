#pragma once

#include "CoreMinimal.h"
#include "SSavableInterface.h"
#include "SWaveLevelState.h"
#include "GameFramework/GameModeBase.h"
#include "SWaveLevelMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*,
                                               KillerController);

UCLASS()
class COOPGAME_API ASWaveLevelMode : public AGameModeBase, public ISSavableInterface
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
	void SpawnNewBot();

	void StartWave();
	void EndWave();
	void PrepareForNextWave();
	void SpawnBotTimerElapsed();
	void CheckWaveState();
	void CheckAnyPlayerAlive();
	void GameOver();
	void SetWaveState(EWaveState NewState) const;
	void RespawnDeadPlayers();

	FTimerHandle TimerHandle_BotSpawn;
	FTimerHandle TimerHandle_NextWaveStart;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float TimeBetweenWaves;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float SpawnRate;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	int32 BotsPerWave;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	int32 WaveCount;

	bool bIsAnyBotAlive;
public:
	ASWaveLevelMode();
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnActorKilled OnActorKilled;
};
