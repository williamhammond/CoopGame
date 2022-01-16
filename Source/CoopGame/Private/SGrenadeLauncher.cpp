#include "SGrenadeLauncher.h"

#include "SGrenade.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
	if (!MyOwner) return;

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

		// FVector ShotDirection = EyeRotation.Vector();

		const FVector MuzzleLocation = MeshComponent->GetSocketLocation(MuzzleSocketName);
		const FRotator MuzzleRotation = MeshComponent->GetSocketRotation(MuzzleSocketName);

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = MyOwner->GetInstigatorController()->GetPawn();
		ActorSpawnParams.Owner = this;

		GetWorld()->SpawnActor<ASGrenade>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		// ASGrenade* grenade = GetWorld()->SpawnActorDeferred<ASGrenade>(ProjectileClass, MuzzleLocation, MuzzleRotation, GetOwner(), GetOwner()->Instigator);
		// grenade->GetProjectileMovement()->bRotationFollowsVelocity = false;
		// grenade->GetProjectileMovement()->SetVelocityInLocalSpace(EyeLocation);
	}
}

void ASGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
