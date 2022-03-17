#include "SGameInstance.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

USGameInstance::USGameInstance()
{
	SaveGameSlotName = "ExampleSlot";
	SaveGameObject = CreateDefaultSubobject<USSaveGame>(TEXT("SaveGame"));;
}

void USGameInstance::LoadGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trying to load a saved game."));

	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	if (LoadedGame)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Game loaded"));
	}
	SaveGameObject = Cast<USSaveGame>(LoadedGame);
	if (!SaveGameObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Saved game"));
	}
}

void USGameInstance::SaveGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saving game..."));

	const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

	if (IsSaved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Game saved."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Cannot save the game."));
	}
}
