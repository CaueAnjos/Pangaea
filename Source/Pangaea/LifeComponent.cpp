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

void ULifeComponent::SetMaxHealth(float NewMax)
{
	if(NewMax > 0)
	{
		_MaxHealth = NewMax;
		if(GetCurrentHealth() > _MaxHealth)
			UpdateCurrentHealth(_MaxHealth);
	}
}

void ULifeComponent::Damage(float damageAmount)
{
	if(!IsDead() && !bIsInvensible && damageAmount > 0)
	{
		float newHealth = GetCurrentHealth() - damageAmount;
		UpdateCurrentHealth(newHealth);
		Server_Damage(damageAmount);
	}
}

void ULifeComponent::Cure(float cureAmount)
{
	if(cureAmount > 0)
	{
		float newHealth = GetCurrentHealth() + cureAmount;
		UpdateCurrentHealth(newHealth);
	}
}

void ULifeComponent::SetCurrentHealth(float NewHealth)
{
	_CurrentHealth = FMath::Clamp(NewHealth, 0.f, _MaxHealth);
	Rep_CurrentHealth();
}

void ULifeComponent::Rep_CurrentHealth()
{
	OnHealthChange.Broadcast(this);

	if(IsDead())
		OnDie.Broadcast(GetOwner(), this);
}

void ULifeComponent::Server_Damage_Implementation(float DamageAmount)
{
	Net_Damage(DamageAmount);
}

void ULifeComponent::Net_Damage_Implementation(float DamageAmount)
{
	OnTakeDamage.Broadcast(GetOwner(), this, DamageAmount);
}

void ULifeComponent::UpdateCurrentHealth_Implementation(float NewHealth)
{
	if(GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		SetCurrentHealth(NewHealth);
	}
}

void ULifeComponent::UpdateMaxHealth_Implementation(float NewMax)
{
	if(GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		SetMaxHealth(NewMax);
	}
}

