#include "SPowerup.h"

#include "TimerManager.h"


ASPowerup::ASPowerup()
{
	PowerupInterval = 0.0f;
	TotalNumberOfTicks = 0.0f;
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
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerup::ActivatePowerup()
{
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerup::OnTickPowerup, PowerupInterval, true,
		                                0.0f);
	}
	else
	{
		OnTickPowerup();
	}
}
