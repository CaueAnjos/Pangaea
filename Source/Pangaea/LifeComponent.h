#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, ULifeComponent*, LifeComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANGAEA_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	ULifeComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditAnywhere, Category = "LifeStatus", meta = (ClampMin = "0"))
	float _MaxHealth = 100;

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float value);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() const
	{
		return _MaxHealth;
	}

	UFUNCTION(BlueprintCallable)
	void Damage(float damageAmount);

	UFUNCTION(BlueprintCallable)
	void Cure(float cureAmount);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDead() const
	{
		return _CurrentHealth <= 0;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() const
	{
		return _CurrentHealth;
	}

private:	
	UPROPERTY(ReplicatedUsing=Rep_CurrentHealth)
	float _CurrentHealth;

	UFUNCTION()
	void Rep_CurrentHealth();

};
