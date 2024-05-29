#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class APlayerCharacter;
class APlayerAIController;
class APlayerCameraManPawn;
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PANGAEA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

	FORCEINLINE APlayerCharacter* GetPlayerCharacter()
	{
		return PlayerCharacter.Get();
	}

	FORCEINLINE APlayerAIController* GetPlayerAIController()
	{
		return AIController.Get();
	}

	void Attack(const AActor* TargetActor);
	void MoveTo(const FVector TargetLocation);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pangaea | Input")
	UNiagaraSystem* SetDestinationFX;

	UPROPERTY(EditDefaultsOnly, Category = "Pangaea | Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pangaea | Input")
	UInputAction* MoveToOrAttackAction;

	void HandleMoveToOrAttack();

	UPROPERTY(EditDefaultsOnly, Category = "Pangaea | Input")
	TSubclassOf<APlayerAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pangaea | Camera")
	TSubclassOf<APlayerCameraManPawn> CameraMamClass;

private:
	TWeakObjectPtr<APlayerCharacter> PlayerCharacter;
	TWeakObjectPtr<APlayerAIController> AIController;
	TWeakObjectPtr<APlayerCameraManPawn> CameraMam;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveTo(const FVector TargetLocation);
	void ServerMoveTo_Implementation(const FVector TargetLocation);
	bool ServerMoveTo_Validate(const FVector TargetLocation);
};
