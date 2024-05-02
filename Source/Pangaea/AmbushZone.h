#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmbushZone.generated.h"

class IZoneEnemy;
class USphereComponent;

UCLASS()
class PANGAEA_API AAmbushZone : public AActor
{
	GENERATED_BODY()

public:	
	AAmbushZone();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "Ambush")
	FORCEINLINE TArray<TScriptInterface<IZoneEnemy>> GetEnemysInZone()
	{
		return EnemysInZone;
	}

	UFUNCTION(BlueprintPure, Category = "Ambush")
	FORCEINLINE TArray<AActor*> GetTriggerActorsInZone()
	{
		return TriggerActorsInZone;
	}

	FORCEINLINE bool IsAmbushTriggered()
	{
		return HasTriggeredAmbush;
	}

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void RegisterEnemysInZone();

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void TryTriggerAmbush(AActor* TriggerActor);

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void EndAmbush();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Ambush")
	USphereComponent* AmbushCircle;

	UPROPERTY(EditAnywhere, Category = "Ambush")
	FName TriggerTag;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastCallEnemysStartAmbush();
	void MulticastCallEnemysStartAmbush_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastCallEnemysStopAmbush();
	void MulticastCallEnemysStopAmbush_Implementation();

private:
	UPROPERTY(Replicated)
	TArray<TScriptInterface<IZoneEnemy>> EnemysInZone;

	UPROPERTY(Replicated)
	TArray<AActor*> TriggerActorsInZone;

	UFUNCTION()
	void OnOverlapAmbushZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapAmbushZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Replicated)
	bool HasTriggeredAmbush;

};
