// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"

void UBrandNewFunctionLibrary::ApplyPrimaryAttributesSetByCaller(
	const FBaseAttributePrams& InAttributePrams, UAbilitySystemComponent* InASC,  const TSubclassOf<UGameplayEffect>& EffectToApply)
{
	if (!InASC || !EffectToApply) return;

	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	ContextHandle.AddSourceObject(InASC->GetAvatarActor());
	
	const FGameplayEffectSpecHandle EffectSpecHandle = InASC->MakeOutgoingSpec(EffectToApply, 1.f, ContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Strength, InAttributePrams.Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Dexterity, InAttributePrams.Dexterity);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Intelligence, InAttributePrams.Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Vitality, InAttributePrams.Vitality);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_MaxHealth, InAttributePrams.MaxHealth);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_CurrentHealth, InAttributePrams.CurrentHealth);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_MaxMana, InAttributePrams.MaxMana);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_CurrentMana, InAttributePrams.CurrentMana);

	InASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
}
