#include "Weapons/SGrenadeLauncher.h"

#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/SGrenade.h"


ASGrenadeLauncher::ASGrenadeLauncher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void ASGrenadeLauncher::Fire()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		return;
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComponent, MuzzleSocketName);
	}

	if (ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		const FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = MyOwner->GetInstigatorController()->GetPawn();
		ActorSpawnParams.Owner = this;

		GetWorld()->SpawnActor<ASGrenade>(ProjectileClass, MuzzleLocation, EyeRotation, ActorSpawnParams);
	}
}

void ASGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
