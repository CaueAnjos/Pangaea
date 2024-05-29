#include "Player/PlayerCameraManPawn.h"
#include "Player/MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCameraManPawn::APlayerCameraManPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->TargetArmLength = 500.f;
	CameraSpringArm->TargetOffset = FVector(0.f, 0.f, 500.f);
	SetRootComponent(CameraSpringArm);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeRotation(FRotator(45.f, 0.f, 0.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCameraManPawn::Initialize(AMyPlayerController* OwnerController, APawn* PlayerPawn)
{
	SetOwner(OwnerController);
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
	AttachToActor(PlayerPawn, AttachmentTransformRules);
}

void APlayerCameraManPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AActor* ParentActor = GetAttachParentActor())
	{
		SetActorRotation(FixedCameraManRotation);
	}
}
