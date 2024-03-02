#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, int, CurrentHealth, int, MaxHealth);

class UAnimMontage;
class UBoxComponent;
class UAvatarAnimInstance;

UCLASS()
class PANGAEA_API AAvatar : public ACharacter
{
	GENERATED_BODY()
public:
	AAvatar();

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Status")
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Status")
	float Strength = 5.f;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Status")
	int Armor = 1;

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

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, ReplicatedUsing = OnHealthChangeRep)
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

	void Hit(int damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DieProcess();
	void DieProcess_Implementation();
};
