#include "Weapons/SGrenade.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASGrenade::ASGrenade()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(50.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASGrenade::OnHit);
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	RootComponent = CollisionComp;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("GrenadeComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	ExplosionRadius = 50.0f;

	InitialLifeSpan = 1.0f;
}

void ASGrenade::BeginPlay()
{
	Super::BeginPlay();
}

void ASGrenade::Explode() const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorTransform());
	const TArray<AActor*> IgnoreActors = {};

	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		return;
	}
	AController* MyInstigator = MyOwner->GetInstigatorController();
	if (!MyInstigator)
	{
		return;
	}

	UGameplayStatics::ApplyRadialDamage(this, 20.0f, GetActorLocation(), ExplosionRadius, DamageType, IgnoreActors,
	                                    MyOwner, MyInstigator, true, ECC_Camera);
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Red, true, 2.0f, 16, 2.0f);
}

void ASGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams( FComponentHitSignature, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit );
void ASGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	if (GetLocalRole() == ROLE_Authority)
	{
		MakeNoise(1.0f, GetInstigator());
		Destroy();
	}
}

void ASGrenade::BeginDestroy()
{
	Explode();
	Super::BeginDestroy();
}

void ASGrenade::LifeSpanExpired()
{
	Explode();
	Super::LifeSpanExpired();
}
