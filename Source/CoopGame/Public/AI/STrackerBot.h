#pragma once

#include "CoreMinimal.h"
#include "SSavableInterface.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class USphereComponent;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn, public ISSavableInterface
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

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USphereComponent* OverlapComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float AggroRadius;

	UPROPERTY()
	AActor* Target;

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

	FTimerHandle TimerHandle_SelfDamage;
	bool bStartedSelfDestruction;

	void DamageSelf();

	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float SelfDamageInterval;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float ExplosionRadius;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float ExplosionDamage;

	bool bExploded;
	
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	USoundCue* SelfDestructSound;
	
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	USoundCue* ExplodeSound;

	FTimerHandle TimerHandle_RefreshPath;
	void RefreshPath();
	void Move();


public:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
