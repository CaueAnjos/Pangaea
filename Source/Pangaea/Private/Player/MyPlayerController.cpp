#include "Player/MyPlayerController.h"
#include "Player/PangaeaPlayerState.h"
#include "Player/PlayerAIController.h"
#include "Player/PlayerCameraManPawn.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputAction.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;

	UEnhancedInputLocalPlayerSubsystem* InputSubsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(InputSubsytem)
	{
		if(DefaultMappingContext)
		{
			InputSubsytem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogInput, Error, TEXT("DefaultMappingContext is NULL"));
		}
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("EnhancedInputLocalPlayerSubsystem is NULL"));
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APlayerCharacter* PlayerCharacterTemp = Cast<APlayerCharacter>(InPawn);
	if(PlayerCharacterTemp)
	{
		PlayerCharacter = PlayerCharacterTemp;

		APangaeaPlayerState* State = GetPlayerState<APangaeaPlayerState>();
		if(State)
		{
			State->GetAbilitySystemComponent()->InitAbilityActorInfo(State, PlayerCharacter.Get());
		}
		else
		{
			UE_LOG(LogPlayerController, Error, TEXT("The PlayerState is not a PangaeaPlayerState"));
		}

		AIController = GetWorld()->SpawnActor<APlayerAIController>(AIControllerClass);
		UnPossess();

		CameraMam = GetWorld()->SpawnActor<APlayerCameraManPawn>(CameraMamClass, PlayerCharacter.Get()->GetActorTransform());
		Possess(CameraMam.Get());

		if(AIController.IsValid() && CameraMam.IsValid())
		{
			AIController->Initialize(this, PlayerCharacter.Get());
			CameraMam->Initialize(this, PlayerCharacter.Get());
		}
		else
		{
			UE_LOG(LogPlayerController, Error, TEXT("The AIController or the CameraMam are nor valid"));
		}
	}
	else if(PlayerCharacter.IsExplicitlyNull())
	{
		UE_LOG(LogPlayerController, Error, TEXT("The possesed pawn is not a PlayerCharacter"));
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveToOrAttackAction, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleMoveToOrAttack);
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("InputComponent is not EnhancedInputComponent"));
	}
}

void AMyPlayerController::Attack(const AActor* TargetActor)
{
	unimplemented();
}

void AMyPlayerController::MoveTo(const FVector TargetLocation)
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerMoveTo(TargetLocation);
	}
	else
	{
		if(PlayerCharacter.IsValid())
		{
			AIController->MoveToLocation(TargetLocation);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SetDestinationFX, TargetLocation);
		}
	}
}

void AMyPlayerController::HandleMoveToOrAttack()
{
	FHitResult Hit;
	bool bHitAnyThing = false;
	bHitAnyThing = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	// Implement the Attack possibility
	if(bHitAnyThing)
	{
		MoveTo(Hit.Location);
	}
}

void AMyPlayerController::ServerMoveTo_Implementation(const FVector TargetLocation)
{
	MoveTo(TargetLocation);
}

bool AMyPlayerController::ServerMoveTo_Validate(const FVector TargetLocation)
{
	return true; // there is no validation at all
}
