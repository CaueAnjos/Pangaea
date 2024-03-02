#include "DefenseTower.h"
#include "PlayerAvatar.h"
#include "Projictile.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "PangaeaGameMode.h"
#include "Kismet/GameplayStatics.h"

ADefenseTower::ADefenseTower()
{
	PrimaryActorTick.bCanEverTick = true;

	_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	_SphereComponent->SetSphereRadius(AttackRange);
	SetRootComponent(_SphereComponent);

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	_MeshComponent->SetupAttachment(_SphereComponent);
}

void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(0.5f);

	_HealthPoints = HealthPoints + (HealthPoints * ShellDefense * 0.25f);

	_SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADefenseTower::OnBeginOverlap);
	_SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADefenseTower::OnEndOverlap);

	_GameMode = Cast<APangaeaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority() && _Target)
		Fire();
}

int ADefenseTower::GetHealthPoints() const
{
	return _HealthPoints;
}

bool ADefenseTower::IsDestroyed() const
{
	return _HealthPoints <= 0;
}

bool ADefenseTower::CanFire() const
{
	return (_ReloadCountingDown <= 0.f);
}

void ADefenseTower::Fire()
{
	if(!ProjictileClass) UE_LOG(LogTemp, Error, TEXT("No projictile class set!"));
	if(HasAuthority())
	{
		auto fireBall = _GameMode->SpawnOrGetFireball(ProjictileClass);

		FVector startLocation = GetActorLocation();
		startLocation.Z += 100.f;
		FVector targetLocation = _Target->GetActorLocation();
		targetLocation.Z = startLocation.Z;

		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(startLocation, targetLocation);

		fireBall->SetActorLocation(startLocation);
		fireBall->SetActorRotation(rotation);
	}
}

void ADefenseTower::Hit(int damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("HIT: Damage = %d"), damage));
	if(damage > 0)
	{
		_HealthPoints -= damage;
		if(_HealthPoints <= 0) DestroyProcess();
	}
}

void ADefenseTower::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<APlayerAvatar>(OtherActor);
	if(player != nullptr) 
		_Target = player;
}

void ADefenseTower::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	if(_Target != nullptr && OtherActor == (void*)_Target)
		_Target = nullptr;
}

void ADefenseTower::OnMeshBeginOverlap(AActor* OtherActor)
{
	auto weapon = Cast<AWeapon>(OtherActor);
	if(weapon != nullptr && weapon->Holder)
	{
		auto playerAvatar = Cast<APlayerAvatar>(weapon->Holder);
		if(playerAvatar != nullptr && playerAvatar->IsAttacking())
			Hit(playerAvatar->Strength);
	}
}

void ADefenseTower::DestroyProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	Destroy();
}

