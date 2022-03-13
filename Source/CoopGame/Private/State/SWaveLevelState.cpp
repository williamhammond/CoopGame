#include "State/SWaveLevelState.h"

#include "UnrealNetwork.h"

void ASWaveLevelState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChange(WaveState, OldState);
}

void ASWaveLevelState::SetWaveState(EWaveState NewState)
{
	if (Role == ROLE_Authority)
	{
		const EWaveState OldState = WaveState;
		WaveState = NewState;
		OnRep_WaveState(OldState);
	}
}

void ASWaveLevelState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASWaveLevelState, WaveState);
}
