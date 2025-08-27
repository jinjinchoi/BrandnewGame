// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BrandNewAbilitySystemGlobals.h"

#include "AbilitySystem/BrandNewAbilityTypes.h"

FGameplayEffectContext* UBrandNewAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBrandNewEffectContext();
}
