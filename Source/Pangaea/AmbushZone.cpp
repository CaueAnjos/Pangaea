#include "AmbushZone.h"
#include "Components/SphereComponent.h"
#include "ZoneEnemy.h"

AAmbushZone::AAmbushZone()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggeredAmbush = false;

	AmbushCircle = CreateDefaultSubobject<USphereComponent>(TEXT("Area"));
	AmbushCircle->InitSphereRadius(200.f);
}

void AAmbushZone::RegisterEnemysInZone()
{
	TArray<FHitResult> Hits;
	bool bHit = GetWorld()->SweepMultiByObjectType(Hits,
		GetActorLocation(), GetActorLocation(), FQuat::Identity,
		FCollisionObjectQueryParams::DefaultObjectQueryParam,
		FCollisionShape::MakeSphere(AmbushCircle->GetScaledSphereRadius()));

	if(bHit)
	{
		for(const FHitResult& Hit : Hits)
		{
			if(Hit.GetActor()->Implements<UZoneEnemy>())
			{
				EnemysInZone.Emplace(Hit.GetActor());
			}
		}
	}

	if(GetEnemysInZone().Num() == 0)
	{
		Destroy();
	}
}

void AAmbushZone::TriggerAmbush(AActor* OverlappedActor, AActor* OtherActor)
{
	TriggeredAmbush = true;

	for(TScriptInterface<IZoneEnemy>& Enemy : GetEnemysInZone())
	{
		IZoneEnemy::Execute_OnStartAmbush(Enemy.GetObject(), this);
	}
}

void AAmbushZone::EndAmbush(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("EndAmbush"));

	TriggeredAmbush = false;
	if(GetEnemysInZone().Num() == 0)
	{
		Destroy();
	}
	else
	{
		for(TScriptInterface<IZoneEnemy>& Enemy : GetEnemysInZone())
		{
			IZoneEnemy::Execute_OnStopAmbush(Enemy.GetObject(), this);
		}
	}
}

void AAmbushZone::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AAmbushZone::TriggerAmbush);
	OnActorEndOverlap.AddDynamic(this, &AAmbushZone::EndAmbush);

	RegisterEnemysInZone();
}

