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
	_LifeComp->bIsInvensible = false;
}

void AAvatar::Attack_BroadCast_Implementation()
{
	GetWorldTimerManager().SetTimer(AttackCoolDownTimer, this, &AAvatar::OnFinishAttackCoolDown, AttackInterval, false, AttackInterval);
	bIsAttacking = true;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Attack"));

	OnAttack.Broadcast();
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
	return bIsAttacking == false && !_LifeComp->IsDead();
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
	if(damage > 0 && !_LifeComp->bIsInvensible)
	{
		_LifeComp->Damage(damage);

		_LifeComp->bIsInvensible = true;
		GetWorldTimerManager().SetTimer(ReHitCoolDownTimer, this, &AAvatar::OnFinishReHitCoolDown, ReHitInterval, false, ReHitInterval);

		if(_LifeComp->IsDead())
			DieProcess(this, _LifeComp);
	}
}
