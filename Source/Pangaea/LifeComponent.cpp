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
	if(GetOwner()->HasAuthority())
	{
		_MaxHealth = value;
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
	if(IsDead())
	{
		OnDie.Broadcast();
	}
}

