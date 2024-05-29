#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlayerAIController.generated.h"

class AMyPlayerController;

UCLASS()
class PANGAEA_API APlayerAIController : public AAIController
{
	GENERATED_BODY()
public:
	void Initialize(AMyPlayerController* OwnerController, APawn* PlayerPawn);

	UFUNCTION(BlueprintPure, Category = "Pangaea | Input")
	FORCEINLINE AMyPlayerController* GetPlayerController()
	{
		return PlayerController.Get();
	}

private:
	TWeakObjectPtr<AMyPlayerController> PlayerController;
	
};
