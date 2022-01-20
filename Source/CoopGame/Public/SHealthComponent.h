#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"


UCLASS(ClassGroup=(COOP), meta=(BlueprintSpawnableComponent))
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USHealthComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="HealthComponent")
	float DefaultHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthComponent")
	float Health;

	UFUNCTION()
	void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                      class AController* InstigatedBy, AActor* DamageCauser);
};
