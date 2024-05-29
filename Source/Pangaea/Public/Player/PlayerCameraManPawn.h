#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCameraManPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMyPlayerController;

UCLASS()
class PANGAEA_API APlayerCameraManPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerCameraManPawn();

	void Initialize(AMyPlayerController* OwnerController, APawn* PlayerPawn);

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Camera")
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Camera")
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pangaea | Camera")
	FRotator FixedCameraManRotation;
};
