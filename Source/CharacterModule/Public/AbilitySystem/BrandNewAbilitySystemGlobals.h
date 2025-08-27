// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "BrandNewAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
