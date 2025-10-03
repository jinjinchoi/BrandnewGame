// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_CriticalChance.h"

float UMMC_CriticalChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParams, Strength);

	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(DexterityDef, Spec, EvaluationParams, Dexterity);
	
	float ItemStrength = 0.f;
	GetCapturedAttributeMagnitude(ItemStrengthDef, Spec, EvaluationParams, ItemStrength);

	float ItemDexterity = 0.f;
	GetCapturedAttributeMagnitude(ItemDexterityDef, Spec, EvaluationParams, ItemDexterity);
	
	const float CriticalChance = FMath::Clamp(5.f + 0.25f * (Dexterity + ItemDexterity) + 0.05f * (Strength + ItemStrength), 0.f, 100.f);
	
	return round(CriticalChance);
}
