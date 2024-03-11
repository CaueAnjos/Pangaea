#include "Weapon.h"
#include "PlayerAvatar.h"
#include "Avatar.h"
#include "LifeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_StaticMesh);
	_StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	_PickUpSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUp Collider"));
	_PickUpSphere->SetupAttachment(RootComponent);
	_PickUpSphere->InitSphereRadius(20.f);
	_PickUpSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnPickUpSphereOverlap);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!HasHolder())
	{
		FQuat rotQuat(FRotator(0, 300.f * DeltaTime, 0));
		AddActorLocalRotation(rotQuat);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, _Holder);
}

void AWeapon::SetHolder(AAvatar* newHolder)
{
	if(IsValid(newHolder))
	{
		if(IsValid(_Holder))
			_Holder->GetLifeComponent()->OnDie.RemoveDynamic(this, &AWeapon::OnHolderDie);

		_Holder = newHolder;
		_Holder->GetLifeComponent()->OnDie.AddDynamic(this, &AWeapon::OnHolderDie);

		SetOwner(newHolder);
		newHolder->Weapon = this;
	}
	else _Holder = nullptr;
}

void AWeapon::DropWeapon()
{
	SetHolder(nullptr);
	_PickUpSphere->SetActive(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DropWeapon_MultCast();
}

void AWeapon::PickUpWeapon(APlayerAvatar* playerAvatar)
{
	SetHolder(playerAvatar);
	_PickUpSphere->SetActive(false);

	TArray<AActor*> attachedActors;
	playerAvatar->GetAttachedActors(attachedActors);

	for(int i = 0; i < attachedActors.Num(); i++)
	{
		AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
		if(weapon)
			weapon->DropWeapon();
	}

	AttachToComponent(GetHolder()->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("hand_rSocket")));
}

void AWeapon::OnHolderDie()
{
	if(HasAuthority())
		DropWeapon();
}

void AWeapon::DropWeapon_MultCast_Implementation()
{
	FVector newPosition = GetActorLocation();
	newPosition.Z = 90.f;
	SetActorLocationAndRotation(newPosition, FQuat::Identity);
}

void AWeapon::OnPickUpSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority() && !HasHolder())
	{
		auto playerAvatar = Cast<APlayerAvatar>(OtherActor);
		if(playerAvatar)
		{
			PickUpWeapon(playerAvatar);
		}
	}
}
