#include "Weapon.h"
#include "PlayerAvatar.h"
#include "Avatar.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_StaticMesh);
	_StaticMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority() && Holder && Holder->IsKilled())
	{
		DropWeapon();
	}

	if(!Holder)
	{
		FQuat rotQuat(FRotator(0, 300.f * DeltaTime, 0));
		AddActorLocalRotation(rotQuat);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, Holder);
}

void AWeapon::DropWeapon()
{
	Holder = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DropWeapon_MultCast();
}

void AWeapon::PickUpWeapon(APlayerAvatar* playerAvatar)
{
	Holder = playerAvatar;
	TArray<AActor*> attachedActors;
	playerAvatar->GetAttachedActors(attachedActors);

	for(int i = 0; i < attachedActors.Num(); i++)
	{
		AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
		if(weapon)
			weapon->DropWeapon();
	}

	AttachToComponent(Holder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("hand_rSocket")));
}

void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	auto avatar = Cast<AAvatar>(OtherActor);
	if(avatar != nullptr)
	{
		if(Holder && avatar != Holder && Holder->IsAttacking())
		{
			avatar->Hit(Holder->Strength + Streegth);
		}
		else if(HasAuthority() && !Holder)
		{
			auto playerAvatar = Cast<APlayerAvatar>(avatar);
			if(playerAvatar)
				PickUpWeapon(playerAvatar);
		}
	}
}

void AWeapon::DropWeapon_MultCast_Implementation()
{
	FVector newPosition = GetActorLocation();
	newPosition.Z = 90.f;
	SetActorLocationAndRotation(newPosition, FQuat::Identity);
}
