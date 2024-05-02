#include "AmbushZone.h"
#include "Components/SphereComponent.h"
#include "ZoneEnemy.h"
#include "Net/UnrealNetwork.h"

AAmbushZone::AAmbushZone()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	HasTriggeredAmbush = false;

	AmbushCircle = CreateDefaultSubobject<USphereComponent>(TEXT("Area"));
	AmbushCircle->InitSphereRadius(200.f);
	AmbushCircle->SetCollisionProfileName(TEXT("Trigger"));
}

void AAmbushZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAmbushZone, EnemysInZone);
	DOREPLIFETIME(AAmbushZone, TriggerActorsInZone);
	DOREPLIFETIME(AAmbushZone, HasTriggeredAmbush);
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

void AAmbushZone::TryTriggerAmbush(AActor* TriggerActor)
{
	if(TriggerActor->ActorHasTag(TriggerTag) && !TriggerActorsInZone.Contains(TriggerActor))
	{
		HasTriggeredAmbush = true;
		TriggerActorsInZone.Add(TriggerActor);
		MulticastCallEnemysStartAmbush();
	}
}

void AAmbushZone::EndAmbush()
{
	HasTriggeredAmbush = false;
	if(GetEnemysInZone().Num() == 0)
		Destroy();
	else
		MulticastCallEnemysStopAmbush();
}

void AAmbushZone::OnOverlapAmbushZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsAmbushTriggered())
	{
		TryTriggerAmbush(OtherActor);		
	}
}

void AAmbushZone::MulticastCallEnemysStartAmbush_Implementation()
{
	for(TScriptInterface<IZoneEnemy>& Enemy : GetEnemysInZone())
	{
		IZoneEnemy::Execute_OnStartAmbush(Enemy.GetObject(), this);
	}
}

void AAmbushZone::MulticastCallEnemysStopAmbush_Implementation()
{
	for(TScriptInterface<IZoneEnemy>& Enemy : GetEnemysInZone())
	{
		IZoneEnemy::Execute_OnStopAmbush(Enemy.GetObject(), this);
	}
}

void AAmbushZone::OnEndOverlapAmbushZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(TriggerActorsInZone.Contains(OtherActor))
	{
		TriggerActorsInZone.Remove(OtherActor);
	}

	if(TriggerActorsInZone.Num() == 0)
	{
		EndAmbush();
	}
}

void AAmbushZone::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		AmbushCircle->OnComponentBeginOverlap.AddDynamic(this, &AAmbushZone::OnOverlapAmbushZone);
		AmbushCircle->OnComponentEndOverlap.AddDynamic(this, &AAmbushZone::OnEndOverlapAmbushZone);

		RegisterEnemysInZone();
	}
}

