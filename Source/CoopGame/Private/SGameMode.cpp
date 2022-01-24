#include "SGameMode.h"

#include "TimerManager.h"


ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;
	SpawnRate = 1.0f;
	WaveCount = 0.0f;
}

void ASGameMode::StartWave()
{
	WaveCount++;
	BotsPerWave = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn, this, &ASGameMode::SpawnBotTimerElapsed, SpawnRate, true, 0.0f);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawn);
	PrepareForNextWave();
}

void ASGameMode::PrepareForNextWave()
{
	FTimerHandle TimerHandle_NextWaveStart;
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	BotsPerWave--;
	if (BotsPerWave <= 0)
	{
		EndWave();
	}
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}
