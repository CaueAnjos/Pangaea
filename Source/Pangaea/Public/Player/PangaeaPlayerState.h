#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PangaeaPlayerState.generated.h"

class UAbilitySystemComponent;
class UPangaeaAttributeSet;

UCLASS()
class PANGAEA_API APangaeaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	APangaeaPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	const UPangaeaAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	const UPangaeaAttributeSet* AttributeSet;
};
