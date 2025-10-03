// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MagicAttackPower.h"

float UMMC_MagicAttackPower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParams, Intelligence);

	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(DexterityDef, Spec, EvaluationParams, Dexterity);

	float ItemIntelligence = 0.f;
	GetCapturedAttributeMagnitude(ItemIntelligenceDef, Spec, EvaluationParams, ItemIntelligence);

	float ItemDexterity = 0.f;
	GetCapturedAttributeMagnitude(ItemDexterityDef, Spec, EvaluationParams, ItemDexterity);
	
	return round(10.f + 3.2f * (Intelligence + ItemIntelligence) + 0.5f * (Dexterity + ItemDexterity));
}
