#include "DefenseTower.h"
#include "PlayerAvatar.h"
#include "Projictile.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "PangaeaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DefenseTower.h"
#include "LifeComponent.h"

ADefenseTower::ADefenseTower()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	_SphereComponent->SetSphereRadius(AttackRange);
	SetRootComponent(_SphereComponent);

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	_MeshComponent->SetupAttachment(_SphereComponent);

	LifeComp = CreateDefaultSubobject<ULifeComponent>(TEXT("Life"));
}

void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(0.5f);

	bEndReHitCoolDown = true;

	LifeComp->OnDie.AddDynamic(this, &ADefenseTower::DestroyProcess);

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

float ADefenseTower::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if(ActualDamage > 0.f && CanTakeDamage())
	{
		GetWorldTimerManager().SetTimer(ReHitCoolDownTimer, this, &ADefenseTower::OnFinishReHitCoolDown, ReHitInterval, false, ReHitInterval);
		bEndReHitCoolDown = false;

		Hit(ActualDamage);
		return ActualDamage;		
	}
	else return 0.f;
}

bool ADefenseTower::CanTakeDamage()
{
	return bEndReHitCoolDown;
}

bool ADefenseTower::IsDestroyed() const
{
	return LifeComp->IsDead();
}

bool ADefenseTower::CanFire() const
{
	return true;
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
	LifeComp->Damage(damage);
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

void ADefenseTower::DestroyProcess(AActor* DieActor, ULifeComponent* DieActorLifeComp)
{
	DieActor->Destroy();
}

void ADefenseTower::OnFinishReHitCoolDown()
{
	GetWorldTimerManager().ClearTimer(ReHitCoolDownTimer);
	bEndReHitCoolDown = true;
}

