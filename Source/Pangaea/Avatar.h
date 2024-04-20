#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

class UAnimMontage;
class UBoxComponent;
class UAvatarAnimInstance;
class AWeapon;
class ULifeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);

UCLASS()
class PANGAEA_API AAvatar : public ACharacter
{
	GENERATED_BODY()
public:
	AAvatar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Params|Status")
	float Strength = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Avatar Params|Attack")
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	float AttackInterval = 3.f;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	float ReHitInterval = 1.f;

	UPROPERTY(BlueprintAssignable)
	FOnAttack OnAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Params|Attack")
	UBoxComponent* HitBox;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAvatarAnimInstance* _AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Status")
	ULifeComponent* _LifeComp;

private:
	uint8 bIsAttacking : 1;

	FTimerHandle AttackCoolDownTimer;
	FTimerHandle ReHitCoolDownTimer;

	void OnFinishAttackCoolDown();
	void OnFinishReHitCoolDown();

	UFUNCTION(NetMulticast, Reliable)
	void Attack_BroadCast();
	void Attack_BroadCast_Implementation();

public:	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar")
	bool IsKilled() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar")
	bool CanAttack() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar")
	bool IsAttacking() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void K2_Attack()
	{
		if(CanAttack())
		{
			Attack();
		}
	}

	UFUNCTION(Server, Reliable)
	void Attack();
	void Attack_Implementation();

	UFUNCTION(BlueprintCallable)
	void Hit(int damage);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DieProcess(AActor* DieActor, ULifeComponent* DieActorLifeComp);

	UFUNCTION(BlueprintPure)
	FORCEINLINE ULifeComponent* GetLifeComponent() const
	{
		return _LifeComp;
	}
};
