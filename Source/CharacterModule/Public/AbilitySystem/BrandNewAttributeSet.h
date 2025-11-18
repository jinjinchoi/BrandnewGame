// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BrandNewAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBrandNewAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

private:
	void HandleIncomingDamage(const struct FGameplayEffectModCallbackData& Data);
	void HandleIncomingXP();
	void ShowDamageText(const struct FGameplayEffectModCallbackData& Data, const float DamageAmount) const;
	void HandleHit(const struct FGameplayEffectModCallbackData& Data, float LocalIncomingDamage) const;
	void HitReportToEnemy(const struct FGameplayEffectModCallbackData& Data, const float LocalIncomingDamage) const;
	void TryActivateDeathReactAbility(const struct FGameplayEffectModCallbackData& Data) const;
	void SendXP(const struct FGameplayEffectModCallbackData& Data) const;
	void IncreaseQuestProgressOnEnemyDeath(AActor* Instigator) const;

public:
	////////////////////////////////////
	////////// Attributes//////////////
	/////////////////////////////////
#pragma region  Vital // 체력 및 마력
	
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ThisClass, Health);

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(ThisClass, Mana);

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);
	
#pragma endregion

#pragma region PrimaryAttribute  // 주속성
	
	UPROPERTY(ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(ThisClass, Strength);

	UPROPERTY(ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(ThisClass, Intelligence);

	UPROPERTY(ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(ThisClass, Dexterity);

	UPROPERTY(ReplicatedUsing = OnRep_Vitality)
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(ThisClass, Vitality);
	
#pragma endregion 

#pragma region SecondaryAttribute
	
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalAttackPower)
	FGameplayAttributeData PhysicalAttackPower; // 물리공격력
	ATTRIBUTE_ACCESSORS(ThisClass, PhysicalAttackPower);

	UPROPERTY(ReplicatedUsing = OnRep_MagicAttackPower)
	FGameplayAttributeData MagicAttackPower; // 마법공격력
	ATTRIBUTE_ACCESSORS(ThisClass, MagicAttackPower);
	
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalDefensePower)
	FGameplayAttributeData PhysicalDefensePower; // 물리방어력
	ATTRIBUTE_ACCESSORS(ThisClass, PhysicalDefensePower);

	UPROPERTY(ReplicatedUsing = OnRep_MagicDefensePower)
	FGameplayAttributeData MagicDefensePower; // 마법방어력
	ATTRIBUTE_ACCESSORS(ThisClass, MagicDefensePower);
	
	UPROPERTY(ReplicatedUsing = OnRep_CriticalChance)
	FGameplayAttributeData CriticalChance; // 치명타 확률
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalChance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalMagnitude)
	FGameplayAttributeData CriticalMagnitude; // 치명타 배율
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalMagnitude);
	

#pragma endregion

#pragma region ExperienceAttribute
	UPROPERTY(ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel; // 레벨
	ATTRIBUTE_ACCESSORS(ThisClass, CharacterLevel);

	UPROPERTY(ReplicatedUsing = OnRep_XP)
	FGameplayAttributeData XP; // 레벨
	ATTRIBUTE_ACCESSORS(ThisClass, XP);

	UPROPERTY(ReplicatedUsing = OnRep_AttributePoint)
	FGameplayAttributeData AttributePoint; // 스탯을 찍을 수 있는 포인트
	ATTRIBUTE_ACCESSORS(ThisClass, AttributePoint);
	
#pragma endregion 
	
#pragma region MetaAttribute
	UPROPERTY()
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingDamage);

	UPROPERTY()
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingXP);
	
#pragma endregion

#pragma region ItemAttribute
	UPROPERTY(ReplicatedUsing = OnRep_ItemStrength)
	FGameplayAttributeData ItemStrength;
	ATTRIBUTE_ACCESSORS(ThisClass, ItemStrength);

	UPROPERTY(ReplicatedUsing = OnRep_ItemIntelligence)
	FGameplayAttributeData ItemIntelligence;
	ATTRIBUTE_ACCESSORS(ThisClass, ItemIntelligence);

	UPROPERTY(ReplicatedUsing = OnRep_ItemDexterity)
	FGameplayAttributeData ItemDexterity;
	ATTRIBUTE_ACCESSORS(ThisClass, ItemDexterity);

	UPROPERTY(ReplicatedUsing = OnRep_ItemVitality)
	FGameplayAttributeData ItemVitality;
	ATTRIBUTE_ACCESSORS(ThisClass, ItemVitality);
#pragma endregion 

private:
#pragma region OnRep_Functions
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);
	
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality);

	UFUNCTION()
	void OnRep_PhysicalAttackPower(const FGameplayAttributeData& OldPhysicalAttackPower);
	
	UFUNCTION()
	void OnRep_MagicAttackPower(const FGameplayAttributeData& OldMagicAttackPower);
	
	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance);
	
	UFUNCTION()
	void OnRep_CriticalMagnitude(const FGameplayAttributeData& OldCriticalMagnitude);
	
	UFUNCTION()
	void OnRep_PhysicalDefensePower(const FGameplayAttributeData& OldPhysicalDefensePower);
	
	UFUNCTION()
	void OnRep_MagicDefensePower(const FGameplayAttributeData& OldMagicDefensePower);

	UFUNCTION()
	void OnRep_CharacterLevel(const FGameplayAttributeData& OldLevel);

	UFUNCTION()
	void OnRep_XP(const FGameplayAttributeData& OldXP);
	
	UFUNCTION()
	void OnRep_AttributePoint(const FGameplayAttributeData& OldAttributePoint);

	UFUNCTION()
	void OnRep_ItemStrength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_ItemIntelligence(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ItemDexterity(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_ItemVitality(const FGameplayAttributeData& OldValue);

#pragma endregion
	
};
