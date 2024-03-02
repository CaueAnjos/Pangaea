#include "EnemyController.h"
#include "Enemy.h"

void AEnemyController::MakeAttackDecision(APawn* targetPawn)
{
	auto controlledCharacter = Cast<AEnemy>(GetPawn());
	double distance = FVector::Dist2D(targetPawn->GetActorLocation(), controlledCharacter->GetTargetLocation());

	if(distance <= controlledCharacter->AttackRange && controlledCharacter->CanAttack())
	{
		controlledCharacter->Attack();
	}
}
