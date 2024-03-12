// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaGameMode.h"
#include "PangaeaPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Projictile.h"

APangaeaGameMode::APangaeaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APangaeaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerAvatar"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

AProjictile* APangaeaGameMode::SpawnOrGetFireball(TSubclassOf<AProjictile> projictileClass)
{
	AProjictile* projictile = nullptr;
	if(_FireballPool.Dequeue(projictile))
		projictile->Reset();
	else
		projictile = GetWorld()->SpawnActor<AProjictile>(projictileClass);

	return projictile;
}

void APangaeaGameMode::RecycleFireBall(AProjictile* projictile)
{
	if(projictile != nullptr)
	{
		projictile->SetActorHiddenInGame(true);
		projictile->SetActorEnableCollision(false);
		projictile->SetActorTickEnabled(false);
		_FireballPool.Enqueue(projictile);
	}
}
