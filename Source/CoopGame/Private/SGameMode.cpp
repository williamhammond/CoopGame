#include "SGameMode.h"

#include "SHealthComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;
	SpawnRate = 1.0f;
	WaveCount = 0.0f;
	bIsAnyBotAlive = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASGameMode::StartWave()
{
	WaveCount++;
	BotsPerWave = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn, this, &ASGameMode::SpawnBotTimerElapsed, SpawnRate, true,
	                                0.0f);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawn);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	bIsAnyBotAlive = true;
	BotsPerWave--;
	if (BotsPerWave <= 0)
	{
		EndWave();
	}
}

void ASGameMode::CheckWaveState()
{
	bool const bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (bIsPreparingForWave || BotsPerWave > 0)
	{
		return;
	}

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* Current = It->Get();
		if (Current && !Current->IsPlayerControlled())
		{
			USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				Current->GetComponentByClass(USHealthComponent::StaticClass()));
			if (HealthComponent && HealthComponent->GetHealth() > 0.0f)
			{
				bIsAnyBotAlive = false;
				break;
			}
		}
	}
	if (bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}
