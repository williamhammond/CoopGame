#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerup;

UCLASS()
class COOPGAME_API ASPickupActor : public AActor
{
	GENERATED_BODY()

public:
	ASPickupActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Componenets")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category="Componenets")
	UDecalComponent* DecalComponent;

	UPROPERTY(EditInstanceOnly, Category="PickupActor")
	TSubclassOf<ASPowerup> PowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category="PickupActor")
	float CooldownDuration;

	FTimerHandle TimerHandle_RespawnTimer;

	UPROPERTY(EditDefaultsOnly, Category="PickupActor")
	ASPowerup* PowerUpInstance;

	void Respawn();

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
