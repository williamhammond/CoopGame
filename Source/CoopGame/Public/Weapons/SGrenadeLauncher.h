// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGrenadeLauncher.generated.h"

class ASGrenade;
UCLASS()
class COOPGAME_API ASGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()

public:
	ASGrenadeLauncher();

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<ASGrenade> ProjectileClass;

public:
	virtual void Tick(float DeltaTime) override;
};
