#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "SSavableInterface.h"
#include "Weapons/SWeapon.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter, public ISSavableInterface
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category="Player", meta=(ClampMin = 0.1, ClampMax = 100.0))
	float ZoomInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player")
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

	UFUNCTION()
	void OnHealthChanged(class USHealthComponent* HealthComp, float
	                     Health, float HealthDelta, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	bool bDied;

	virtual void OnActorLoaded_Implementation() override;

public:
	UFUNCTION(BlueprintCallable, Category="Player")
	void StartFire();
	
	UFUNCTION(BlueprintCallable, Category="Player")
	void StopFire();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
};
