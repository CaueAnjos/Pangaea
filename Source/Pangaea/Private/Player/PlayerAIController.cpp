#include "Player/PlayerAIController.h"
#include "Player/MyPlayerController.h"

void APlayerAIController::Initialize(AMyPlayerController* OwnerController, APawn* PlayerPawn)
{
	if(OwnerController)
	{
		SetOwner(OwnerController);
		PlayerController = OwnerController;
		Possess(PlayerPawn);
	}
	else
	{
		UE_LOG(LogController, Error, TEXT("Controller is not valid"));
	}
}
