#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

UCLASS()
class COOPGAME_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Spectating")
	TSubclassOf<AActor> SpectatingViewpointClass;

	UFUNCTION(BlueprintNativeEvent, Category="GameMode")
	bool IsMissionSuccess();

	UFUNCTION(BlueprintNativeEvent, Category="GameMode")
	bool IsMissionFailure();

	virtual bool IsMissionSuccess_Implementation();
	virtual bool IsMissionFailure_Implementation();

public:
	ASGameModeBase();

	UFUNCTION(BlueprintCallable, Category="GameMode")
	void CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccess);

	virtual void Tick(float DeltaTime) override;
};
