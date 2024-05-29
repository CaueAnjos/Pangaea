#include "Player/PangaeaPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/PangaeaAttributeSet.h"

APangaeaPlayerState::APangaeaPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPangaeaAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* APangaeaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

const UPangaeaAttributeSet* APangaeaPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
