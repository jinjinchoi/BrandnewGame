// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_ManaRegeneration.h"
#include "AbilitySystem/BrandNewAttributeSet.h"

UMMC_ManaRegeneration::UMMC_ManaRegeneration()
{
	MaxManaDef.AttributeToCapture = UBrandNewAttributeSet::GetStrengthAttribute();
	MaxManaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxManaDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(MaxManaDef);
}

float UMMC_ManaRegeneration::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;
	
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParams, Intelligence);

	float ItemIntelligence = 0.f;
	GetCapturedAttributeMagnitude(ItemIntelligenceDef, Spec, EvaluationParams, ItemIntelligence);

	float MaxMana = 0.f;
	GetCapturedAttributeMagnitude(MaxManaDef, Spec, EvaluationParams, MaxMana);

	return round(0.04f * MaxMana + 0.1f * (Intelligence + ItemIntelligence)); 
	
}
