#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projictile.generated.h"

class APangaeaGameMode;

UCLASS(Blueprintable)
class PANGAEA_API AProjictile : public AActor
{
	GENERATED_BODY()
public:	
	AProjictile();

	UPROPERTY(EditAnywhere, Category = "Projictile Params")
	float Speed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Projictile Params")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "Projictile Params")
	float Damage = 10.f;

protected:
	virtual void BeginPlay() override;

	APangaeaGameMode* _GameMode;

	FTimerHandle LifeSpanTimer;
	void OnLifeSpanEnd();

public:	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

	void Reset();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;
};
