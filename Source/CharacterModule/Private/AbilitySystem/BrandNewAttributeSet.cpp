// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BrandNewAttributeSet.h"

#include "DebugHelper.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"

UBrandNewAttributeSet::UBrandNewAttributeSet()
{
	
}

void UBrandNewAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PhysicalAttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MagicAttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalMagnitude, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PhysicalDefensePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MagicDefensePower, COND_None, REPNOTIFY_Always);
	
}

void UBrandNewAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	
}

void UBrandNewAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Data);
	}
}

void UBrandNewAttributeSet::HandleIncomingDamage(const struct FGameplayEffectModCallbackData& Data)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage <= 0.f) return;
	if (GetHealth() <= 0.f) return;

	const float NewHealth = FMath::Clamp(GetHealth() - LocalIncomingDamage, 0.f, GetMaxHealth());
	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

	if (NewHealth <= 0.f)
	{
		// Death Logic
	}
	else
	{
		// Hit Logic
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(BrandNewGamePlayTag::Ability_Shared_React_Hit);
		Data.Target.TryActivateAbilitiesByTag(TagContainer);
	}
	
	
}


#pragma region OnRep_Functions

void UBrandNewAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
}

void UBrandNewAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

void UBrandNewAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

void UBrandNewAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}

void UBrandNewAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UBrandNewAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UBrandNewAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Dexterity, OldDexterity);
}

void UBrandNewAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vitality, OldVitality);
}

void UBrandNewAttributeSet::OnRep_PhysicalAttackPower(const FGameplayAttributeData& OldPhysicalAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PhysicalAttackPower, OldPhysicalAttackPower);
}

void UBrandNewAttributeSet::OnRep_MagicAttackPower(const FGameplayAttributeData& OldMagicAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MagicAttackPower, OldMagicAttackPower);
}

void UBrandNewAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalChance, OldCriticalChance);
}

void UBrandNewAttributeSet::OnRep_CriticalMagnitude(const FGameplayAttributeData& OldCriticalMagnitude)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalMagnitude, OldCriticalMagnitude);
}

void UBrandNewAttributeSet::OnRep_PhysicalDefensePower(const FGameplayAttributeData& OldPhysicalDefensePower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PhysicalDefensePower, OldPhysicalDefensePower);
}

void UBrandNewAttributeSet::OnRep_MagicDefensePower(const FGameplayAttributeData& OldMagicDefensePower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MagicDefensePower, OldMagicDefensePower);
}
#pragma endregion 