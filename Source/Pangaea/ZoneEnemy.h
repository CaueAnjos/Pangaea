#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AmbushZone.h"
#include "ZoneEnemy.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UZoneEnemy : public UInterface
{
	GENERATED_BODY()
};

class PANGAEA_API IZoneEnemy
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ambush")
	void OnStartAmbush(AAmbushZone* Ambush);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ambush")
	void OnStopAmbush(AAmbushZone* Ambush);
};
