#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

class AAvatar;
class APlayerAvatar;
class USphereComponent;

UCLASS()
class PANGAEA_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Params")
	float Streegth = 10;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	AAvatar* _Holder = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* _StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* _PickUpSphere;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SetHolder(AAvatar* newHolder);

	UFUNCTION(BlueprintPure)
	FORCEINLINE AAvatar* GetHolder() const
	{
		return _Holder;
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool HasHolder() const
	{
		return _Holder != nullptr;
	}

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

	UFUNCTION(BlueprintCallable)
	void PickUpWeapon(APlayerAvatar* playerAvatar);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool HitDetection(TArray<FHitResult>& hit);

private:
	UFUNCTION()
	void OnHolderDie(AActor* DieActor, ULifeComponent* DieActorLifeComp);

	UFUNCTION(Server, Unreliable)
	void Server_DropWeapon();
	void Server_DropWeapon_Implementation();

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_PickUp(APlayerAvatar* playerAvatar);
	void Server_PickUp_Implementation(APlayerAvatar* playerAvatar);
	bool Server_PickUp_Validate(APlayerAvatar* playerAvatar);

	UFUNCTION()
	void OnPickUpSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
