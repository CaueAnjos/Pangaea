#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.h"
#include "PlayerAvatar.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public AAvatar
{
	GENERATED_BODY()
public:
	APlayerAvatar();

public:	
	FORCEINLINE UCameraComponent* GetCameraComponent() const
	{
		return _CameraComponent;
	}

	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const
	{
		return _SpringArmComponent;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _SpringArmComponent;
};
