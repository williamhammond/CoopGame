#pragma once

#include "CoreMinimal.h"
#include "SSaveGame.h"
#include "Engine/GameInstance.h"

#include "SGameInstance.generated.h"

UCLASS()
class COOPGAME_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USGameInstance();

	UPROPERTY(BlueprintReadOnly)
	FString SaveGameSlotName;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	USSaveGame* SaveGameObject;

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void SaveGame();
};
