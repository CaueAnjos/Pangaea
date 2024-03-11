#include "LifeComponent.h"

ULifeComponent::ULifeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void ULifeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

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

