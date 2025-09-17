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

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeNonTargetEffectParams() const;
	
	UPROPERTY(EditDefaultsOnly,  Category="Brandnew|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 물리공격, 마법공격을 구분하는 태그
	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Damage", meta = (Categories = "Damage.Type"))
	FGameplayTag DamageType;

	// 공격의 속성을 구분하는 태그
	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Damage", meta = (Categories = "Damage.Element"))
	FGameplayTag DamageElement;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Damage", meta=(ClampMin="0.0"))
	FScalableFloat DamageCoefficient;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Damage")
	float KnockbackForce = 200.f;
	
};
