#pragma once

#include "CoreMinimal.h"
#include "Character/PangaeaCharacterBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PANGAEA_API APlayerCharacter : public APangaeaCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void Die() override;

	virtual void FinishDying() override;

};
