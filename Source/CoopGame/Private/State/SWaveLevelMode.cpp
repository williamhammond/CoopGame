#include "State/SWaveLevelMode.h"

#include "AIController.h"
#include "SHealthComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Player/SPlayerState.h"


ASWaveLevelMode::ASWaveLevelMode()
{
	TimeBetweenWaves = 2.0f;

	GameStateClass = ASWaveLevelState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	SpawnRate = 1.0f;
	WaveCount = 0.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASWaveLevelMode::StartWave()
{
	WaveCount++;
	BotsPerWave = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawn, this, &ASWaveLevelMode::SpawnBotTimerElapsed, SpawnRate, true,
	                                0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASWaveLevelMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawn);

	SetWaveState(EWaveState::WaitingToComplete);
}

void ASWaveLevelMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASWaveLevelMode::StartWave, TimeBetweenWaves,
	                                false);
	RespawnDeadPlayers();
	SetWaveState(EWaveState::WaitingToStart);
}

void ASWaveLevelMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	BotsPerWave--;
	if (BotsPerWave <= 0)
	{
		EndWave();
	}
}

void ASWaveLevelMode::CheckWaveState()
{
	const bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	if (bIsPreparingForWave || BotsPerWave > 0)
	{
		return;
	}

	bIsAnyBotAlive = false;
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const AAIController* Current = Cast<AAIController>(It->Get());
		if (Current)
		{
			const auto Pawn = Current->GetPawn();
			const USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				Pawn->GetComponentByClass(USHealthComponent::StaticClass()));
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

void ASWaveLevelMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const APlayerController* PlayerController = It->Get();
		const APawn* MyPawn = PlayerController->GetPawnOrSpectator();
		if (PlayerController && MyPawn)
		{
			const USHealthComponent* HealthComponent = Cast<USHealthComponent>(
				MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComponent) && HealthComponent->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	GameOver();
}

void ASWaveLevelMode::GameOver()
{
	EndWave();
	SetWaveState(EWaveState::GameOver);
	UE_LOG(LogTemp, Log, TEXT("Game over players died"));
}

void ASWaveLevelMode::SetWaveState(EWaveState NewState) const
{
	ASWaveLevelState* WaveState = GetGameState<ASWaveLevelState>();
	if (ensureAlways(WaveState))
	{
		WaveState->SetWaveState(NewState);
	}
}

void ASWaveLevelMode::RespawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		const APawn* MyPawn = PlayerController->GetPawnOrSpectator();
		if (PlayerController && !MyPawn)
		{
			RestartPlayer(PlayerController);
		}
	}
}

void ASWaveLevelMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASWaveLevelMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();
}
