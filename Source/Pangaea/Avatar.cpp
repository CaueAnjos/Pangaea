#include "Avatar.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "AvatarAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Weapon.h"
#include "LifeComponent.h"

AAvatar::AAvatar()
{
 	PrimaryActorTick.bCanEverTick = true;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box Collider"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->InitBoxExtent(FVector(50.f, 50.f, 80.f));

	_LifeComp = CreateDefaultSubobject<ULifeComponent>(TEXT("Life"));
	_LifeComp->SetMaxHealth(500.f);
}

void AAvatar::BeginPlay()
{
	Super::BeginPlay();
	bIsAttacking = false;
	bEndReHitCoolDown = true;

	//_LifeComp->OnDie.AddDynamic(this, &AAvatar::DieProcess);

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

float AAvatar::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if(ActualDamage > 0.f)
	{
		Hit(ActualDamage);
		return ActualDamage;
	}
	else return 0.f;
}

bool AAvatar::IsKilled() const
{
	return _LifeComp->IsDead();
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

		_LifeComp->Damage(damage);

		GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);	

		if(_LifeComp->IsDead())
			DieProcess(this, _LifeComp);
	}
}
