#include "Avatar.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "AvatarAnimInstance.h"
#include "Net/UnrealNetwork.h"

AAvatar::AAvatar()
{
 	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box Collider"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->InitBoxExtent(FVector(50.f, 50.f, 80.f));
}

void AAvatar::BeginPlay()
{
	Super::BeginPlay();
	bIsAttacking = false;
	bEndReHitCoolDown = true;

	_HealthPoints = HealthPoints + (_HealthPoints * Armor * 0.25);

	_AnimInstance = Cast<UAvatarAnimInstance>(GetMesh()->GetAnimInstance());
}

void AAvatar::OnFinishAttackCoolDown()
{
	GetWorldTimerManager().ClearTimer(AttackCoolDownTimer);
	bIsAttacking = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("OnFinishCoolDown"));
}

void AAvatar::OnFinishReHitCoolDown()
{
	GetWorldTimerManager().ClearTimer(ReHitCoolDownTimer);
	bEndReHitCoolDown = true;
}

void AAvatar::Attack_BroadCast_Implementation()
{
	GetWorldTimerManager().SetTimer(AttackCoolDownTimer, this, &AAvatar::OnFinishAttackCoolDown, AttackInterval, false, AttackInterval);
	bIsAttacking = true;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Attack"));

	if(AttackMontage != nullptr)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}
}

void AAvatar::OnHealthChangeRep()
{
	OnHealthChange.Broadcast(_HealthPoints, HealthPoints);
}

void AAvatar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAvatar, _HealthPoints);
}

int AAvatar::GetHealthPoints() const
{
	return _HealthPoints;
}

bool AAvatar::IsKilled() const
{
	return (_HealthPoints <= 0.f);
}

bool AAvatar::CanAttack() const
{
	return bIsAttacking == false;
}

bool AAvatar::IsAttacking() const
{
	return bIsAttacking;
}

void AAvatar::Attack_Implementation()
{
	Attack_BroadCast();
}

void AAvatar::Hit(int damage)
{
	if(damage > 0 && bEndReHitCoolDown)
	{
		bEndReHitCoolDown = false;
		GetWorldTimerManager().SetTimer(ReHitCoolDownTimer, this, &AAvatar::OnFinishReHitCoolDown, ReHitInterval, false, ReHitInterval);

		if(HasAuthority())
		{
			_HealthPoints -= damage;
			OnHealthChangeRep();
		}

		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);

		if(IsKilled())
			DieProcess();
	}
}

void AAvatar::DieProcess_Implementation()
{
	Destroy();
}
