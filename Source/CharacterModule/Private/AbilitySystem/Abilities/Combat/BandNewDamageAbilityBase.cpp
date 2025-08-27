// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/BandNewDamageAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"

FDamageEffectParams UBandNewDamageAbilityBase::MakeDamageEffectParams(AActor* TargetActor) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageCoefficient = DamageCoefficient.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DamageElement = DamageElement;

	if (IsValid(TargetActor))
	{
		DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	}

	return DamageEffectParams;
	
}
