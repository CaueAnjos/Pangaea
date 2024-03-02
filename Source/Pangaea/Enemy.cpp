#include "Enemy.h"
#include "Perception\PawnSensingComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "AvatarAnimInstance.h"
#include "EnemyController.h"
#include "Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if(_WeaponClass)
	{
		_Weapon = GetWorld()->SpawnActor<AWeapon>(_WeaponClass);
		_Weapon->Holder = this;
		_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
	}

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::Chase);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(_AnimInstance != nullptr)
	{
		if(_ChasedTarget != nullptr && _AnimInstance->AvatarState == EAvatarState::Locomotion)
		{
			auto enemyController = Cast<AEnemyController>(GetController());
			if(enemyController != nullptr)
			{
				enemyController->MakeAttackDecision(_ChasedTarget);
			}
		}
	}
}

void AEnemy::Chase(APawn* targetPawn)
{
	if(_AnimInstance != nullptr)
	{
		if(targetPawn != nullptr && _AnimInstance->AvatarState == EAvatarState::Locomotion)
		{
			auto enemyController = Cast<AEnemyController>(GetController());
			enemyController->MoveToActor(targetPawn, 90.f);

		}
	}
	_ChasedTarget = targetPawn;
}
