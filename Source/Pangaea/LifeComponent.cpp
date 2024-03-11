#include "LifeComponent.h"
#include "Net/UnrealNetwork.h"

ULifeComponent::ULifeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetOwner()->HasAuthority())
	{
		_CurrentHealth = _MaxHealth;
		Rep_CurrentHealth();
	}
}

void ULifeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULifeComponent, _CurrentHealth);
	DOREPLIFETIME(ULifeComponent, _MaxHealth);
}

void ULifeComponent::SetMaxHealth(float value)
{
	if(GetOwner()->HasAuthority() && value > 0)
	{
		_MaxHealth = value;

		if(_CurrentHealth > _MaxHealth)
			_CurrentHealth = _MaxHealth;
	}
}

void ULifeComponent::Damage(float damageAmount)
{
	if(GetOwner()->HasAuthority() && !IsDead()  && damageAmount >= 0)
	{
		_CurrentHealth -= damageAmount;
		Rep_CurrentHealth();
	}
}

void ULifeComponent::Cure(float cureAmount)
{
	if(GetOwner()->HasAuthority() && !IsDead() && cureAmount >= 0)
	{
		_CurrentHealth += cureAmount;
		Rep_CurrentHealth();
	}
}

void ULifeComponent::Rep_CurrentHealth()
{
	OnHealthChange.Broadcast(this);

	if(IsDead())
	{
		OnDie.Broadcast();
	}
}

