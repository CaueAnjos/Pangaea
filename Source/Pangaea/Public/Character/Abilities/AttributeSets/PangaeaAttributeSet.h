#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PangaeaAttributeSet.generated.h"

#define ATRIBUTE_ACCESSOR(ClassName, AttributeName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, AttributeName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(AttributeName)\
GAMEPLAYATTRIBUTE_VALUE_SETTER(AttributeName)\
GAMEPLAYATTRIBUTE_VALUE_INITTER(AttributeName)\

UCLASS()
class PANGAEA_API UPangaeaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(ReplicatedUsing = Rep_Health, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData Health;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, Health)

	UPROPERTY(ReplicatedUsing = Rep_MaxHealth, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData MaxHealth;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, MaxHealth)

	UPROPERTY(ReplicatedUsing = Rep_HealthRegen, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData HealthRegen;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, HealthRegen)

	UPROPERTY(ReplicatedUsing = Rep_Mana, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData Mana;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, Mana)

	UPROPERTY(ReplicatedUsing = Rep_MaxMana, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData MaxMana;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, MaxMana)

	UPROPERTY(ReplicatedUsing = Rep_ManaRegen, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData ManaRegen;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, ManaRegen)

	UPROPERTY(ReplicatedUsing = Rep_Level, EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData Level;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, Level)

	//meta attribute
	UPROPERTY(BlueprintReadOnly, Category = "Pangaea | Abilities")
	FGameplayAttributeData Damage;
	ATRIBUTE_ACCESSOR(UPangaeaAttributeSet, Damage)

protected:
	UFUNCTION()
	virtual void Rep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void Rep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void Rep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);

	UFUNCTION()
	virtual void Rep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void Rep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void Rep_ManaRegen(const FGameplayAttributeData& OldManaRegen);

	UFUNCTION()
	virtual void Rep_Level(const FGameplayAttributeData& OldLevel);

};
