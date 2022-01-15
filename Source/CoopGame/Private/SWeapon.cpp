// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ASWeapon::ASWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	FHitResult Hit;
	bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
	if (isHit)
	{
		
	}
	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
