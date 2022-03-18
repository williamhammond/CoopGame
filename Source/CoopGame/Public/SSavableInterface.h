#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SSavableInterface.generated.h"

UINTERFACE(MinimalAPI)
class USSavableInterface : public UInterface
{
	GENERATED_BODY()
};

class COOPGAME_API ISSavableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();
	
};
