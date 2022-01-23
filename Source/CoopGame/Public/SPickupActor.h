#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class USphereComponent;
class UDecalComponent;

UCLASS()
class COOPGAME_API ASPickupActor : public AActor
{
	GENERATED_BODY()

public:
	ASPickupActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category="Componenets")
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Componenets")
	UDecalComponent* DecalComponent;
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
