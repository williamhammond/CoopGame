#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "SWeapon.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Magnitude);
	void MoveRight(float Magnitude);

	void BeginCrouch();
	void EndCrouch();

	void Zoom();
	void EndZoom();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category="Player", meta=(ClampMin = 0.1, ClampMax = 100.0))
	float ZoomInterpSpeed;

	bool bWantsToZoom;

	float DefaultFOV;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Player")
	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Player")
	FName WeaponAttachSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Player")
	USHealthComponent* HealthComponent;

	void StartFire();
	void StopFire();

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* HealthComp, float
	                     Health, float HealthDelta, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool bDied;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
};
