// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_PhysicalAttackPower.h"


float UMMC_PhysicalAttackPower::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
	
	return round(10.f + 3.f * Strength + Dexterity);
}
