#include "Character/PangaeaCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/PangaeaAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"

DEFINE_LOG_CATEGORY(LogPangaeaCharacter);

#define ATTRIBUTE_GETTER_IMPLEMENTATION(Attribute)\
float AttributeValue = 0.f;\
if(AttributeSet.IsValid())\
{\
	AttributeValue = AttributeSet->Get ## Attribute();\
}\
else\
{\
	UE_LOG(LogPangaeaCharacter, Warning, TEXT("AttributeSet is not set or not valid"));\
}\
return AttributeValue;\

APangaeaCharacterBase::APangaeaCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* APangaeaCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool APangaeaCharacterBase::IsAlive() const
{
	return GetHealth() > 0.f;
}

void APangaeaCharacterBase::ApplyInitialEffect()
{
	if(AbilitySystemComponent.IsValid())
	{
		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);

		FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributeInitialValueEffect, 1.f, Context);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
	else
	{
		UE_LOG(LogPangaeaCharacter, Warning, TEXT("AbilitySystemComponent is not set or not valid"));
	}
}

void APangaeaCharacterBase::GiveInitialAbilities()
{}

void APangaeaCharacterBase::Die()
{
	RemoveAllAbilities();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->ComponentVelocity = FVector::Zero();

	OnCharacterDie.Broadcast(this);

	if(DeathMontage)
		PlayAnimMontage(DeathMontage);
	else
		UE_LOG(LogPangaeaCharacter, Warning, TEXT("DeathMontage is not set"));

	FinishDying();
}

void APangaeaCharacterBase::FinishDying()
{
	Destroy();
}

float APangaeaCharacterBase::GetHealth() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(Health);
}

float APangaeaCharacterBase::GetMaxHealth() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(MaxHealth);
}

float APangaeaCharacterBase::GetHealthRegen() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(HealthRegen);
}

float APangaeaCharacterBase::GetMana() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(Mana);
}

float APangaeaCharacterBase::GetMaxMana() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(MaxMana);
}

float APangaeaCharacterBase::GetManaRegen() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(ManaRegen);
}

float APangaeaCharacterBase::GetCharacterLevel() const
{
	ATTRIBUTE_GETTER_IMPLEMENTATION(Level);
}

void APangaeaCharacterBase::RemoveAllAbilities()
{
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ClearAllAbilities();
	}
	else
	{
		UE_LOG(LogPangaeaCharacter, Warning, TEXT("AbilitySystemComponent is not set or not valid"));
	}
}

void APangaeaCharacterBase::InitAbilitySystemComponent(UAbilitySystemComponent* ASC, const UPangaeaAttributeSet* AttSet)
{
	AbilitySystemComponent = ASC;
	AttributeSet = AttSet;

	ApplyInitialEffect();
	GiveInitialAbilities();
}
