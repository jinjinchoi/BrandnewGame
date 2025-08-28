// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;
	
	float Strength = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParams, Strength);

	float Vitality = 0.0f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParams, Vitality);
	
	return round(100.f + 30.f * Vitality + 5 * Strength);
}
