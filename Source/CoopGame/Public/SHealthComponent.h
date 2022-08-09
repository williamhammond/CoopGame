#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, class USHealthComponent*, HealthComp, float,
                                             Health, float, HealthDelta, const class UDamageType*, DamageType,
                                             class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup=(COOP), meta=(BlueprintSpawnableComponent))
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="HealthComponent")
	uint8 TeamNum;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponent")
	float DefaultHealth;

	UFUNCTION()
	void HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                      class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION()
	void OnRep_Health(float OldHealth);

	bool bIsDead;

public:
	float GetHealth() const;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="HealthComponent")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category="HealthComponent")
	static bool IsFriendly(AActor* A, AActor* B);
};
