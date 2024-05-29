#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PangaeaCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPangaeaCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDieDelegate, APangaeaCharacterBase*, Character);

class UAbilitySystemComponent;
class UPangaeaAttributeSet;
class UAnimMontage;

UCLASS()
class PANGAEA_API APangaeaCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APangaeaCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character")
	bool IsAlive() const;

	virtual void ApplyInitialEffect();
	virtual void GiveInitialAbilities();

	virtual void Die();

	virtual void FinishDying();

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetHealthRegen() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetManaRegen() const;

	UFUNCTION(BlueprintPure, Category = "Pangaea | Character | Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Pangaea | Character | Abilities")
	void RemoveAllAbilities();

	UPROPERTY(BlueprintAssignable, Category = "Pangaea | Character")
	FCharacterDieDelegate OnCharacterDie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pangaea | Character")
	FText CharacterName;

protected:
	virtual void InitAbilitySystemComponent(UAbilitySystemComponent*, const UPangaeaAttributeSet*);

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<const UPangaeaAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Pangaea | Character | Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Pangaea | Character | Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilitiesClasses;

	UPROPERTY(EditAnywhere, Category = "Pangaea | Character | Abilities")
	TSubclassOf<class UGameplayEffect> AttributeInitialValueEffect;

};
