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
	FORCEINLINE TArray<APlayerController*> GetPlayersInZone()
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
	TArray<APlayerController*> PlayersInZone;

	UFUNCTION()
	void TriggerAmbush(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndAmbush(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(Replicated)
	bool HasTriggeredAmbush;

};
