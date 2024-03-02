#include "PlayerAvatar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Avatar.h"

APlayerAvatar::APlayerAvatar()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* characterMovemt = GetCharacterMovement();
	characterMovemt->bOrientRotationToMovement = true;
	characterMovemt->RotationRate = FRotator(0.f, 640.f, 0.f);
	characterMovemt->bConstrainToPlane = true;
	characterMovemt->bSnapToPlaneAtStart = true;

	// Create SpringArm
	_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_SpringArmComponent->SetupAttachment(RootComponent);
	_SpringArmComponent->SetUsingAbsoluteRotation(true);
	_SpringArmComponent->TargetArmLength = 800.f;
	_SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	_SpringArmComponent->bDoCollisionTest = false;

	// Create Camera
	_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_CameraComponent->SetupAttachment(_SpringArmComponent, USpringArmComponent::SocketName);
	_CameraComponent->bUsePawnControlRotation = false;
}
