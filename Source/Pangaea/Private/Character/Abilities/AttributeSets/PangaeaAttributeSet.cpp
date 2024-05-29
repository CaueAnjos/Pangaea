#include "Character/Abilities/AttributeSets/PangaeaAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Character/PangaeaCharacterBase.h"

void UPangaeaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPangaeaAttributeSet, Level, COND_None, REPNOTIFY_Always);
}

void UPangaeaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UPangaeaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageDone = GetDamage();
		SetDamage(0.f);

		APangaeaCharacterBase* TargetCharacter =  Cast<APangaeaCharacterBase>(Data.Target.AbilityActorInfo->AvatarActor);
		if(TargetCharacter && TargetCharacter->IsAlive())
		{
			const float NewHealth = FMath::Clamp(GetHealth() - DamageDone, 0.f, GetMaxHealth());
			SetHealth(NewHealth);
		}
	}
}

void UPangaeaAttributeSet::Rep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, Health, OldHealth);
}

void UPangaeaAttributeSet::Rep_MaxHealth(const FGameplayAttributeData & OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, MaxHealth, OldMaxHealth);
}

void UPangaeaAttributeSet::Rep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, HealthRegen, OldHealthRegen);
}

void UPangaeaAttributeSet::Rep_Mana(const FGameplayAttributeData & OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, Mana, OldMana);
}

void UPangaeaAttributeSet::Rep_MaxMana(const FGameplayAttributeData & OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, MaxMana, OldMaxMana);
}

void UPangaeaAttributeSet::Rep_ManaRegen(const FGameplayAttributeData& OldManaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, ManaRegen, OldManaRegen);
}

void UPangaeaAttributeSet::Rep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPangaeaAttributeSet, Level, OldLevel);
}
