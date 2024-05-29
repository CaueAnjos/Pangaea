#include "Character/PlayerCharacter.h"
#include "Player/PangaeaPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(true);
	bReplicates = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APangaeaPlayerState* State = GetPlayerState<APangaeaPlayerState>();
	if(State)
	{
		InitAbilitySystemComponent(State->GetAbilitySystemComponent(), State->GetAttributeSet());
	}
	else
	{
		UE_LOG(LogPangaeaCharacter, Error, TEXT("PlayerState is NULL or not PangaeaPlayerState"));
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APangaeaPlayerState* State = GetPlayerState<APangaeaPlayerState>();
	if(State)
	{
		InitAbilitySystemComponent(State->GetAbilitySystemComponent(), State->GetAttributeSet());
	}
	else
	{
		UE_LOG(LogPangaeaCharacter, Error, TEXT("PlayerState is not PangaeaPlayerState"));
	}
}

void APlayerCharacter::Die()
{
	Super::Die();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("PlayerCharacter die"));
}

void APlayerCharacter::FinishDying()
{
	Super::FinishDying();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("PlayerCharacter finish dying"));
}
