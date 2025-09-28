// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Combat/BandNewDamageAbilityBase.h"
#include "ImpactAtTargetLocationAbility.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UImpactAtTargetLocationAbility : public UBandNewDamageAbilityBase
{
	GENERATED_BODY()

public:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	
protected:
	UFUNCTION(BlueprintCallable, Category = "Brandnew|AbilityFunction")
	void SpawnWarningEffect();
	
	UFUNCTION(BlueprintCallable, Category = "Brandnew|AbilityFunction")
	void AttackAtTargetLocation();

	/* 적을 포착할 수 있는 최대 범위 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Combat")
	float AbilityRange = 2000.f;

	/* 실제 공격시 대미지가 적용될 범위 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Combat")
	float DamageRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Combat")
	float DamageOriginOffsetZ = 50.f;
	
	/* 경고 이펙트 생성할 게임플레이 큐 태그 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Effect", meta = (Categories = "GameplayCue"))
	FGameplayTag WarningEffectTag;
	
	/*  경고 이펙트 크기 배율 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Effect")
	float WarningEffectMagnitude = 1.f;

	/* 공격 이펙트 생성할 게임플레이 큐 태그 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Effect", meta = (Categories = "GameplayCue"))
	FGameplayTag AttackEffectTag;
	
	/* 공격 이펙트 크기 배율 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Effect")
	float AttackEffectMagnitude = 1.f;


private:
	void UpdateTargetLocationArray();
	void GetValidLocation(const FVector& OriginalLocation, FVector& OutValidLocation, const float OffsetZ = 5.f) const;
	
	TArray<FVector> TargetLocations;
	TQueue<TArray<FVector>> CachedTargetLocationQueue;
	
};
