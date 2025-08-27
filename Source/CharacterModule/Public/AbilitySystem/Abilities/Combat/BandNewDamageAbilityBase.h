// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BandNewBaseGameplayAbility.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "BandNewDamageAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBandNewDamageAbilityBase : public UBandNewBaseGameplayAbility
{
	GENERATED_BODY()



protected:
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 물리공격, 마법공격을 구분하는 태그
	UPROPERTY(EditDefaultsOnly, Category="Damage", meta = (Categories = "Damage.Type"))
	FGameplayTag DamageType;

	// 공격의 속성을 구분하는 태그
	UPROPERTY(EditDefaultsOnly, Category="Damage", meta = (Categories = "Damage.Element"))
	FGameplayTag DamageElement;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(ClampMin="0.0"))
	FScalableFloat DamageCoefficient;
	
};
