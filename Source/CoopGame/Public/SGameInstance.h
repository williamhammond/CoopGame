#pragma once

#include "CoreMinimal.h"
#include "SSaveGame.h"
#include "Engine/GameInstance.h"

#include "SGameInstance.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class USSaveGame*, SaveObject);

UCLASS()
class COOPGAME_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USGameInstance();

	UPROPERTY(BlueprintReadOnly)
	FString SlotName;

	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	USSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void LoadGame();

	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void SaveGame();
	
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;
	
};
