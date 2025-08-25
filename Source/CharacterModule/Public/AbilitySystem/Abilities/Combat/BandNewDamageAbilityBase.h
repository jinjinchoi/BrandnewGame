// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BandNewBaseGameplayAbility.h"
#include "BandNewDamageAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBandNewDamageAbilityBase : public UBandNewBaseGameplayAbility
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta = (Categories = "Damage"))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(ClampMin="0.0"))
	FScalableFloat DamageMultiplier;
	
};
