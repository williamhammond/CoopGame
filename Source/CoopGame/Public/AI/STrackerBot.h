#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USHealthComponent* HealthComponent;

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* OwningHealthComp, float
	                      Health, float HealthDelta, const class UDamageType* DamageType,
	                      class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float MovementForce;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float RequiredDistanceToTarget;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(VisibleDefaultsOnly, Category="TrackerBot")
	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	UParticleSystem* ExplosionEffect;
	void SelfDestruct();

	bool bExploded;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float ExplosionRadius;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float ExplosionDamage;


public:
	virtual void Tick(float DeltaTime) override;
};
