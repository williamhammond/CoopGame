#include "SPowerup.h"

#include "TimerManager.h"
#include "UnrealNetwork.h"


ASPowerup::ASPowerup()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0.0f;
	bIsPowerupActive = false;
	SetReplicates(true);
}

void ASPowerup::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerup::OnTickPowerup()
{
	TickCount++;
	OnPowerupTicked();

	if (TickCount >= TotalNumberOfTicks)
	{
		bIsPowerupActive = false;
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerup::ActivatePowerup(AActor* OtherActor)
{
	OnActivated(OtherActor);
	
	bIsPowerupActive = true;
	OnRep_PowerupActivated();
	
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerup::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void ASPowerup::OnRep_PowerupActivated()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

void ASPowerup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ASPowerup, bIsPowerupActive);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
