#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, int, CurrentHealth, int, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAvatarDie);

class UAnimMontage;
class UBoxComponent;
class UAvatarAnimInstance;
class AWeapon;

UCLASS()
class PANGAEA_API AAvatar : public ACharacter
{
	GENERATED_BODY()
public:
	AAvatar();

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Status")
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Params|Status")
	float Strength = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar Params|Status")
	int Armor = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Avatar Params|Attack")
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	float AttackInterval = 3.f;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	float ReHitInterval = 1.f;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar Params|Attack")
	UBoxComponent* HitBox;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAvatarDie OnAvatarDie;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnHealthChangeRep)
	int _HealthPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAvatarAnimInstance* _AnimInstance;

private:
	uint8 bIsAttacking : 1;
	uint8 bEndReHitCoolDown : 1;

	FTimerHandle AttackCoolDownTimer;
	FTimerHandle ReHitCoolDownTimer;

	void OnFinishAttackCoolDown();
	void OnFinishReHitCoolDown();

	UFUNCTION(NetMulticast, Reliable)
	void Attack_BroadCast();
	void Attack_BroadCast_Implementation();

	UFUNCTION()
	void OnHealthChangeRep();

public:	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar", meta = (DisplayName = "GetHP"))
	int GetHealthPoints() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar")
	bool IsKilled() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar")
	bool CanAttack() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea|PlayerAvatar")
	bool IsAttacking() const;

	UFUNCTION(Server, Reliable)
	void Attack();
	void Attack_Implementation();

	UFUNCTION(BlueprintCallable)
	void Hit(int damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DieProcess();
	void DieProcess_Implementation();
};
