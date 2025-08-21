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

#pragma region  Vital // begin Vital Attribute
	
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
	
#pragma endregion // end Vital Attribute

#pragma region PrimaryAttribute // begin PrimaryAttribute
	
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
	
#pragma endregion  // end PrimaryAttribute

	UPROPERTY(ReplicatedUsing = OnRep_PhysicalAttackPower)
	FGameplayAttributeData PhysicalAttackPower;
	ATTRIBUTE_ACCESSORS(ThisClass, PhysicalAttackPower);

	UPROPERTY(ReplicatedUsing = OnRep_MagicAttackPower)
	FGameplayAttributeData MagicAttackPower;
	ATTRIBUTE_ACCESSORS(ThisClass, MagicAttackPower);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalChance)
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalChance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalMagnitude)
	FGameplayAttributeData CriticalMagnitude;
	ATTRIBUTE_ACCESSORS(ThisClass, CriticalMagnitude);

	UPROPERTY(ReplicatedUsing = OnRep_PhysicalDefensePower)
	FGameplayAttributeData PhysicalDefensePower;
	ATTRIBUTE_ACCESSORS(ThisClass, PhysicalDefensePower);

	UPROPERTY(ReplicatedUsing = OnRep_MagicDefensePower)
	FGameplayAttributeData MagicDefensePower;
	ATTRIBUTE_ACCESSORS(ThisClass, MagicDefensePower);

private:
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
	
};
