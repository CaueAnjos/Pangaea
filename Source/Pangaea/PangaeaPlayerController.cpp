#include "PangaeaPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "PlayerAvatar.h"
#include "InputAction.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APangaeaPlayerController::APangaeaPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void APangaeaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APangaeaPlayerController::PawnLookTo_Implementation(FVector point)
{
	FVector LookDirection = (point - GetPawn()->GetActorLocation()).GetSafeNormal();
	FQuat rotation = LookDirection.Rotation().Quaternion();
	GetPawn()->SetActorRotation(rotation);
}

void APangaeaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &APangaeaPlayerController::OnSetDestinationTriggered);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APangaeaPlayerController::Move);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APangaeaPlayerController::OnAttackTriggered);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APangaeaPlayerController::OnAttackTriggered()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	APawn* ControlledPawn = GetPawn();
	if(ControlledPawn != nullptr)
	{
		FVector point = Hit.Location;
		point.Z = ControlledPawn->GetActorLocation().Z;

		PawnLookTo(point);

		auto playerAvatar = Cast<APlayerAvatar>(GetPawn());
		if(playerAvatar && playerAvatar->CanAttack())
		{
			playerAvatar->Attack();
		}
	}
}

void APangaeaPlayerController::OnSetDestinationTriggered()
{	
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if(bHitSuccessful)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.Location);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXCursor, Hit.Location);
	}
}

void APangaeaPlayerController::Move(const FInputActionInstance& Instance)
{
	FVector2D Direction = Instance.GetValue().Get<FVector2D>();
	if(Direction != FVector2D::Zero())
	{
		GetPawn()->AddMovementInput(FVector(Direction, 0), 1.0, false);
	}
}
