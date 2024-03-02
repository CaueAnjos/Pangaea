#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

class AAvatar;
class APlayerAvatar;

UCLASS()
class PANGAEA_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	AAvatar* Holder = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon Params")
	float Streegth = 10;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* _StaticMesh;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

	UFUNCTION(BlueprintCallable)
	void PickUpWeapon(APlayerAvatar* playerAvatar);

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UFUNCTION(NetMulticast, Unreliable)
	void DropWeapon_MultCast();
	void DropWeapon_MultCast_Implementation();
};
