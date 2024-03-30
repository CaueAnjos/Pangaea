#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PangaeaGameMode.generated.h"

class AProjictile;

UCLASS(minimalapi)
class APangaeaGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	APangaeaGameMode();

	UFUNCTION(BlueprintCallable, Category = "FireBallPool")
	AProjictile* SpawnOrGetFireball(TSubclassOf<AProjictile> projictileClass);

	UFUNCTION(BlueprintCallable, Category = "FireBallPool")
	void RecycleFireBall(AProjictile* projictile);

private:
	TQueue<AProjictile*> _FireballPool;
};



