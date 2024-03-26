#include "LifeComponent.h"
#include "Net/UnrealNetwork.h"

ULifeComponent::ULifeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}


void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetCurrentHealth(_MaxHealth);
}

void ULifeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULifeComponent, _CurrentHealth);
	DOREPLIFETIME(ULifeComponent, _MaxHealth);
	DOREPLIFETIME(ULifeComponent, bIsInvensible);
}

void ULifeComponent::SetMaxHealth(float value)
{
	if(GetOwner()->HasAuthority() && value > 0)
	{
		_MaxHealth = value;

		if(GetCurrentHealth() > _MaxHealth)
			SetCurrentHealth(_MaxHealth);
	}
}

void ULifeComponent::Damage(float damageAmount)
{
	if(!IsDead() && !bIsInvensible && damageAmount > 0)
	{
		float newHealth = GetCurrentHealth() - damageAmount;
		SetCurrentHealth(newHealth);
	}
}

void ULifeComponent::Cure(float cureAmount)
{
	if(cureAmount > 0)
	{
		float newHealth = GetCurrentHealth() + cureAmount;
		SetCurrentHealth(newHealth);
	}
}

void ULifeComponent::SetCurrentHealth(float newHealth)
{
	if(GetOwner()->HasAuthority())
	{
		_CurrentHealth = FMath::Clamp(newHealth, 0.f, _MaxHealth);
		Rep_CurrentHealth();
	}
}

void ULifeComponent::Rep_CurrentHealth()
{
	OnHealthChange.Broadcast(this);

	if(IsDead())
		OnDie.Broadcast(GetOwner(), this);
}

