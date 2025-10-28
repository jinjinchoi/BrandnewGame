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
	/* 대미지를 줄 타겟이 이미 설정된 어빌리티에서 사용하는 파라메터 구조체를 반환하는 함수 */
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParams(AActor* TargetActor) const;

	/* 아직 공격 대상이 확정되지 않은 어빌리티에서 사용. Target Ability System이나 Hit Direction이 설정되지 않음 */
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeNonTargetEffectParams() const;

	/* 특정 지역에 있는 액터에게 대미지를 주는 어빌리티에서 사용. Hit 방향을 Avatar가 아닌 AttackLocation을 기준으로 설정함 */
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeLocationDamageAbilityDamageParams(AActor* TargetActor, const FVector& AttackLocation) const;
	
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
