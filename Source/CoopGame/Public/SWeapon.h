// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class USoundBase;
class UAnimSequence;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASWeapon();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	USkeletalMeshComponent* MeshComponent;

	UFUNCTION(BlueprintCallable, Category="Weapon")

	void PlayFireEffects(FVector TracerEndpoint);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* DefaultImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* FleshImpactEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	USoundBase* FireSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<UCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float BaseDamage;

	// RPM - Bullets per minute fired
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float RateOfFire;
	float TimeBetweenShots;

	FTimerHandle TimerHandle_TimeBetween_Shots;

	float LastFireTime;
	
public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void Fire();

	void StartFire();

	void StopFire();
};
