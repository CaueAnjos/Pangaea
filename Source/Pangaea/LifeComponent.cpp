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
	DOREPLIFETIME(ULifeComponent, _Defense);
}

void ULifeComponent::SetMaxHealth(float value)
{

}

void ULifeComponent::SetDefense(float value)
{

}

void ULifeComponent::Damage(float damageAmount)
{

}

void ULifeComponent::Cure(float cureAmount)
{

}

void ULifeComponent::Rep_CurrentHealth()
{

}

