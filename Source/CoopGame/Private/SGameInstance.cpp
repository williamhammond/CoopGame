#include "SGameInstance.h"

USGameInstance::USGameInstance()
{
}

void USGameInstance::LoadGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Load game command issued"));
}

void USGameInstance::SaveGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Save game command issued"));
}
