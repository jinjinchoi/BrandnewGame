// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MagicDefensePower.h"

float UMMC_MagicDefensePower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Vitality = 0.0f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParams, Vitality);
	
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParams, Intelligence);

	float ItemVitality = 0.0f;
	GetCapturedAttributeMagnitude(ItemVitalityDef, Spec, EvaluationParams, ItemVitality);
	
	float ItemIntelligence = 0.f;
	GetCapturedAttributeMagnitude(ItemIntelligenceDef, Spec, EvaluationParams, ItemIntelligence);

	
	return round(5.f + 0.5f * (Vitality + ItemVitality) + 0.2f * (Intelligence + ItemIntelligence));
	
	
}
