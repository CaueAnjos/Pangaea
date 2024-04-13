#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "PangaeaPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
struct FInputActionInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM()
enum class EWalkMode : uint8
{
	Auto, Manualy
};

UCLASS()
class APangaeaPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APangaeaPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UFUNCTION(Server, Unreliable)
	void PawnLookTo(FVector point);
	void PawnLookTo_Implementation(FVector point);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	/** Input handlers for SetDestination action. */
	void OnSetDestinationTriggered();
	void Move(const FInputActionInstance& Instance);
	void OnAttackTriggered();

	UFUNCTION(BlueprintPure, Category = "Movement")
	FORCEINLINE EWalkMode GetWalkMode()
	{
		return _Walk;
	}

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetWalkMode(EWalkMode WalkMode);

private:
	EWalkMode _Walk;

	UFUNCTION(Server, Unreliable)
	void Server_MoveTo(FVector FinalLocation);
	void Server_MoveTo_Implementation(FVector FinalLocation);

	UFUNCTION(Server, Unreliable)
	void Server_StopMovement();
	void Server_StopMovement_Implementation();
};


