// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_PhysicalDefensePower.h"

float UMMC_PhysicalDefensePower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Vitality = 0.0f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParams, Vitality);

	float Strength = 0.0f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParams, Strength);

	float ItemVitality = 0.0f;
	GetCapturedAttributeMagnitude(ItemVitalityDef, Spec, EvaluationParams, ItemVitality);

	float ItemStrength = 0.0f;
	GetCapturedAttributeMagnitude(ItemStrengthDef, Spec, EvaluationParams, ItemStrength);
	
	return round(5.f + 0.5f * (Vitality + ItemVitality) + 0.2f * (Strength + ItemStrength));
}
