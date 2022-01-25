// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase 
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
	ASGameMode();
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
