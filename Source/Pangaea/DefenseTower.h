#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

class APangaeaGameMode;
class AProjictile;
class ULifeComponent;

UCLASS(Blueprintable)
class PANGAEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()
public:	
	ADefenseTower();

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	ULifeComponent* LifeComp;
	
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float AttackRange = 15.f;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float ReloadInterval = 1.f;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float ReHitInterval = 1.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjictile> ProjictileClass = nullptr;

protected:
	virtual void BeginPlay() override;

	class APlayerAvatar* _Target;

	APangaeaGameMode* _GameMode;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool CanTakeDamage();

	UFUNCTION(BlueprintPure, Category = "Pangaea|Defense Tower")
	bool IsDestroyed() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea|Defense Tower")
	bool CanFire() const;

	void Fire();
	void Hit(int damage);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	FORCEINLINE USphereComponent* GetBoxComponent() const
	{
		return _SphereComponent;
	}

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

protected:
	UFUNCTION()
	void DestroyProcess();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* _SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;

	uint8 bEndReHitCoolDown : 1;

	FTimerHandle ReHitCoolDownTimer;

	void OnFinishReHitCoolDown();
};
