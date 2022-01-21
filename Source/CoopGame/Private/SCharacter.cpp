#include "SCharacter.h"

#include "CoopGame.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponenet"));

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 50.0f;

	WeaponAttachSocketName = "WeaponSocket";
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::Zoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CameraComponent->FieldOfView;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator,
	                                                 SpawnParameters);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                                 WeaponAttachSocketName);
	}
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float Magnitude)
{
	AddMovementInput(GetActorForwardVector() * Magnitude);
}

void ASCharacter::MoveRight(float Magnitude)
{
	AddMovementInput(GetActorRightVector() * Magnitude);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::Zoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	if (CurrentWeapon) CurrentWeapon->StartFire();
}

void ASCharacter::StopFire()
{
	if (CurrentWeapon) CurrentWeapon->StopFire();
}

void ASCharacter::OnHealthChanged(class USHealthComponent* OwningHealthComp, float
                                  Health, float HealthDelta, const class UDamageType* DamageType,
                                  class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		bDied = true;

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float const TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float const NewFOV = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComponent->SetFieldOfView(NewFOV);
}
