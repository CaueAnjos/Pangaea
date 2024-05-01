#include "AmbushZone.h"
#include "Components/SphereComponent.h"
#include "ZoneEnemy.h"
#include "Net/UnrealNetwork.h"

AAmbushZone::AAmbushZone()
{
	PrimaryActorTick.bCanEverTick = true;
	HasTriggeredAmbush = false;

	AmbushCircle = CreateDefaultSubobject<USphereComponent>(TEXT("Area"));
	AmbushCircle->InitSphereRadius(200.f);
}

void AAmbushZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAmbushZone, EnemysInZone);
	DOREPLIFETIME(AAmbushZone, PlayersInZone);
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

void AAmbushZone::TriggerAmbush(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsAmbushTriggered())
	{
		HasTriggeredAmbush = true;
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if(PlayerPawn)
		{
			APlayerController* Player = Cast<APlayerController>(PlayerPawn->GetController());
			if(Player)
			{
				PlayersInZone.Add(Player);
				Net_CallEnemysStartAmbush();
			}
		}
	}
}

void AAmbushZone::Net_CallEnemysStartAmbush_Implementation()
{
	for(TScriptInterface<IZoneEnemy>& Enemy : GetEnemysInZone())
	{
		IZoneEnemy::Execute_OnStartAmbush(Enemy.GetObject(), this);
	}
}

void AAmbushZone::Net_CallEnemysStopAmbush_Implementation()
{
	for(TScriptInterface<IZoneEnemy>& Enemy : GetEnemysInZone())
	{
		IZoneEnemy::Execute_OnStopAmbush(Enemy.GetObject(), this);
	}
}

void AAmbushZone::EndAmbush(AActor* OverlappedActor, AActor* OtherActor)
{
	if(IsAmbushTriggered())
	{
		HasTriggeredAmbush = false;
		if(GetEnemysInZone().Num() == 0)
		{
			Destroy();
		}
		else
		{
			Net_CallEnemysStopAmbush();
		}
	}
}

void AAmbushZone::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority)
	{
		OnActorBeginOverlap.AddDynamic(this, &AAmbushZone::TriggerAmbush);
		OnActorEndOverlap.AddDynamic(this, &AAmbushZone::EndAmbush);

		RegisterEnemysInZone();
	}
}

