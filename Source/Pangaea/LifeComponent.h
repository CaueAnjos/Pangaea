#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"


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

	UPROPERTY(Replicated, EditAnywhere, Category = "LifeStatus", meta = (ClampMin = "0"))
	float _Defense = 10;

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float value);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHealth() const
	{
		return _MaxHealth;
	}

	UFUNCTION(BlueprintCallable)
	void SetDefense(float value);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetDefense() const
	{
		return _Defense;
	}

	UFUNCTION(BlueprintCallable)
	void Damage(float damageAmount);

	UFUNCTION(BlueprintCallable)
	void Cure(float cureAmount);

private:	
	UPROPERTY(ReplicatedUsing=Rep_CurrentHealth)
	float _CurrentHealth;

	UFUNCTION()
	void Rep_CurrentHealth();

};
