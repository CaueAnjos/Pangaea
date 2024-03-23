#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PangaeaGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimerChangeDelegate, float, Timer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameWinOrLose, bool, Win);

UCLASS()
class PANGAEA_API APangaeaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnTimerChange, BlueprintReadWrite, Category = "Time")
	float Timer;

	UPROPERTY(BlueprintAssignable)
	FOnTimerChangeDelegate OnTimerChangeDelegate;

	UFUNCTION(BlueprintCallable)
	void OnTimerChange();

	UFUNCTION(BlueprintCallable)
	void OnGameWin();

	UPROPERTY(ReplicatedUsing = OnGameWin, BlueprintReadWrite)
	bool bGameWin = false;

	UPROPERTY(BlueprintAssignable)
	FOnGameWinOrLose OnGameWinOrLoseDelegate;
};
