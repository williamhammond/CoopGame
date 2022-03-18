#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<uint8> ByteData;
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerID;

	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	float PersonalRecordTime;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	bool bResumeAtTransform;
};

UCLASS()
class COOPGAME_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	USSaveGame();

	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayers;

	/* Actors stored from a level (currently does not support a specific level and just assumes the demo map) */
	UPROPERTY()
	TArray<FActorSaveData> SavedActors;

	UPROPERTY(BlueprintReadWrite, Category="Player Info")
	FString PlayerName;
};
