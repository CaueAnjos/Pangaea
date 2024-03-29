#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

class ULifeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDie, AActor*, DieActor, ULifeComponent*, DieActorLifeComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, ULifeComponent*, LifeComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamage, AActor*, DamageActor, ULifeComponent*, LifeComp, float, DamageAmount);

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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "LifeStatus")
	bool bIsInvensible = false;

	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage OnTakeDamage;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewMax);

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

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth() const
	{
		return _CurrentHealth;
	}

private:	
	UPROPERTY(Replicated, ReplicatedUsing=Rep_CurrentHealth)
	float _CurrentHealth = 0.f;

	UFUNCTION()
	void Rep_CurrentHealth();

	UFUNCTION(Server, Unreliable)
	void Server_Damage(float DamageAmount);
	void Server_Damage_Implementation(float DamageAmount);

	UFUNCTION(NetMulticast, Unreliable)
	void Net_Damage(float DamageAmount);
	void Net_Damage_Implementation(float DamageAmount);

	UFUNCTION(Server, Unreliable)
	void Server_SetCurrentHealth(float NewHealth);
	void Server_SetCurrentHealth_Implementation(float NewHealth);

	UFUNCTION(Server, Unreliable)
	void Server_SetMaxHealth(float NewMax);
	void Server_SetMaxHealth_Implementation(float NewMax);

};
