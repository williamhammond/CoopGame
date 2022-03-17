#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "SSaveGame.generated.h"

UCLASS()
class COOPGAME_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	USSaveGame();
	UPROPERTY(BlueprintReadWrite, Category="Player Info")
	FString PlayerName;
};
