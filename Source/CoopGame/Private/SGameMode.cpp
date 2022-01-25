#include "SGameMode.h"

#include "SGameState.h"
#include "SHealthComponent.h"
#include "SPlayerState.h"
#include "TimerManager.h"
#include "AI/STrackerBot.h"
#include "Engine/World.h"


ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;

	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
	
	SpawnRate = 1.0f;
	WaveCount = 0.0f;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASGameMode::StartWave()
{
	WaveCount++;
	BotsPerWave = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn, this, &ASGameMode::SpawnBotTimerElapsed, SpawnRate, true,
	                                0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawn);

	SetWaveState(EWaveState::WaitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
	RespawnDeadPlayers();
	SetWaveState(EWaveState::WaitingToStart);
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

void ASGameMode::CheckWaveState()
{
	bool const bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (bIsPreparingForWave || BotsPerWave > 0)
	{
		return;
	}

	bIsAnyBotAlive = false;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* Current = Cast<ASTrackerBot>(It->Get());
		if (Current)
		{
			USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				Current->GetComponentByClass(USHealthComponent::StaticClass()));
			if (HealthComponent && HealthComponent->GetHealth() > 0.0f)
			{
				bIsAnyBotAlive = true;
				break;
			}
		}
	}
	if (!bIsAnyBotAlive)
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		APawn* MyPawn = PlayerController->GetPawnOrSpectator();
		if (PlayerController && MyPawn)
		{
			USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComponent) && HealthComponent->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();
	SetWaveState(EWaveState::GameOver);
	UE_LOG(LogTemp, Log, TEXT("Game over players died"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GameState = GetGameState<ASGameState>();
	if (ensureAlways(GameState))
	{
		GameState->SetWaveState(NewState);	
	}
}

void ASGameMode::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		APawn* MyPawn = PlayerController->GetPawnOrSpectator();
		if (PlayerController && !MyPawn)
		{
			RestartPlayer(PlayerController);
		}
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
	CheckAnyPlayerAlive();
}
