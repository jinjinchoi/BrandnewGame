// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_HealthRegeneration.h"

#include "AbilitySystem/BrandNewAttributeSet.h"

UMMC_HealthRegeneration::UMMC_HealthRegeneration()
{
	MaxHealthDef.AttributeToCapture = UBrandNewAttributeSet::GetStrengthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxHealthDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UMMC_HealthRegeneration::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	float MaxHealth = 0.f;
	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluationParams, MaxHealth);

	return round(  0.002f * MaxHealth + 0.1 * (Vitality + ItemVitality) + 0.05 * (Strength + ItemStrength));
	
}
