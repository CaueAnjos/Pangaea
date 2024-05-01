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
		return TriggeredAmbush;
	}

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void RegisterEnemysInZone();

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void TriggerAmbush(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Ambush")
	void EndAmbush(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

	bool TriggeredAmbush;

	UPROPERTY(EditInstanceOnly, Category = "Ambush")
	USphereComponent* AmbushCircle;

private:
	TArray<TScriptInterface<IZoneEnemy>> EnemysInZone;

	TArray<APlayerController*> PlayersInZone;

};
