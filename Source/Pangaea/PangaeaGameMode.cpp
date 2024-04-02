// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaGameMode.h"
#include "PangaeaPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Projictile.h"
#include "PangaeaGameState.h"

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

	GameStateClass = APangaeaGameState::StaticClass();
}

AProjictile* APangaeaGameMode::SpawnOrGetFireball(TSubclassOf<AProjictile> projectileClass)
{
    AProjictile* projectile = nullptr;
    _FireballPool.Dequeue(projectile);
    if(IsValid(projectile))
    {
        projectile->SetActorHiddenInGame(false);
    }
    else if(projectileClass)
    {
       projectile = GetWorld()->SpawnActor<AProjictile>(projectileClass);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid projectile class!"));
    }
    return projectile;
}

void APangaeaGameMode::RecycleFireBall(AProjictile* projectile)
{
    if(IsValid(projectile))
    {
        projectile->SetActorHiddenInGame(true);
        _FireballPool.Enqueue(projectile);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to recycle a null projectile!"));
    }
}

