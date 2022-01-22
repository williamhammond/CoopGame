#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	FVector GetNextPathPoint();
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float MovementForce;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	float RequiredDistanceToTarget;
	UPROPERTY(EditDefaultsOnly, Category="TrackerBot")
	bool bUseVelocityChange;
	

public:	
	virtual void Tick(float DeltaTime) override;
};
