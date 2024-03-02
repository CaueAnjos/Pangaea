#include "Projictile.h"
#include "PlayerAvatar.h"
#include "Kismet\GameplayStatics.h"
#include "PangaeaGameMode.h"

AProjictile::AProjictile()
{
	PrimaryActorTick.bCanEverTick = true;

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_MeshComponent);
}

void AProjictile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);

	_GameMode = Cast<APangaeaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &AProjictile::OnLifeSpanEnd, LifeSpan, false, LifeSpan);
}

void AProjictile::OnLifeSpanEnd()
{
	_GameMode->RecycleFireBall(this);
}

void AProjictile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector currentLocation = GetActorLocation();
	FVector velocity = GetActorRotation().RotateVector(FVector::ForwardVector) * Speed * DeltaTime;
	FVector nextLocation = currentLocation + velocity;

	SetActorLocation(nextLocation);


	FHitResult hitResult;
	FCollisionObjectQueryParams params;
	params.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	if(GetWorld()->LineTraceSingleByObjectType(hitResult, currentLocation, nextLocation, params))
	{
		auto playerAvatar = Cast<APlayerAvatar>(hitResult.GetActor());
		if(HasAuthority() && playerAvatar != nullptr)
		{
			playerAvatar->Hit(Damage);
			_GameMode->RecycleFireBall(this);
		}
	}
}

void AProjictile::Reset()
{
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &AProjictile::OnLifeSpanEnd, LifeSpan, false, LifeSpan);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

