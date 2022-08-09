#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "STrackerBotController.generated.h"

UCLASS()
class COOPGAME_API ASTrackerBotController : public AAIController
{
	GENERATED_BODY()
public:
	ASTrackerBotController();
	UFUNCTION()
	AActor* GetTarget() const;

protected:
	UFUNCTION()
	void HandlePerceptionUpdated(const TArray<AActor*>& UpdatedActors);


	UPROPERTY()
	AActor* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAISenseConfig_Hearing* HearingConfig;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	FGenericTeamId TeamId;
};
