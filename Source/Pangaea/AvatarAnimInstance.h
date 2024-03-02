#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AvatarAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EAvatarState : uint8
{
	Locomotion = 0,
	Attack,
	Hit,
	Die
};

class AAvatar;

UCLASS()
class PANGAEA_API UAvatarAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAvatarState AvatarState;

	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();

	UFUNCTION(BlueprintPure)
	AAvatar* GetOwningAvatar();
};
