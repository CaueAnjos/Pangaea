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
	FORCEINLINE TArray<APawn*> GetPlayersInZone()
	{
		return PlayersInZone;
	}

	FORCEINLINE bool IsAmbushTriggered()
	{
		return HasTriggeredAmbush;
	}

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void RegisterEnemysInZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Ambush")
	USphereComponent* AmbushCircle;

	UPROPERTY(EditAnywhere, Category = "Ambush")
	FName TriggerTag;

	UFUNCTION(NetMulticast, Reliable)
	void Net_CallEnemysStartAmbush();
	void Net_CallEnemysStartAmbush_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Net_CallEnemysStopAmbush();
	void Net_CallEnemysStopAmbush_Implementation();

private:
	UPROPERTY(Replicated)
	TArray<TScriptInterface<IZoneEnemy>> EnemysInZone;

	UPROPERTY(Replicated)
	TArray<APawn*> PlayersInZone;

	UFUNCTION()
	void TriggerAmbush(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndAmbush(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Replicated)
	bool HasTriggeredAmbush;

};
