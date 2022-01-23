#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerup.generated.h"

UCLASS()
class COOPGAME_API ASPowerup : public AActor
{
	GENERATED_BODY()

public:
	ASPowerup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Powerups")
	float PowerupInterval;

	UPROPERTY(EditDefaultsOnly, Category="Powerups")
	int32 TotalNumberOfTicks;

	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
	void OnTickPowerup();

	int32 TickCount;

	UFUNCTION()
	void OnRep_PowerupActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPowerupStateChanged(bool bIsNewActive);

public:
	void ActivatePowerup(AActor* OtherActor);
	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnActivated(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnExpired();
	
	UPROPERTY(ReplicatedUsing=OnRep_PowerupActivated)
	bool bIsPowerupActive;

};
