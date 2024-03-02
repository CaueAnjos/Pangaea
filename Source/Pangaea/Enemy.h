#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Avatar.h"
#include "Enemy.generated.h"

class AWeapon;

UCLASS()
class PANGAEA_API AEnemy : public AAvatar
{
	GENERATED_BODY()
public:
	AEnemy();

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	float AttackRange = 25.f;

protected:
	virtual void BeginPlay() override;

	APawn* _ChasedTarget = nullptr;

	UPROPERTY(EditAnywhere, Category = "Avatar Params|Attack")
	TSubclassOf<AWeapon> _WeaponClass;

	AWeapon* _Weapon;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Enemy")
	void Chase(APawn* targetPawn);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PawnSensingComponent;
};
