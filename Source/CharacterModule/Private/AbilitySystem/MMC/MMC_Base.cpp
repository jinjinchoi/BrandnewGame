// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_Base.h"

#include "AbilitySystem/BrandNewAttributeSet.h"

UMMC_Base::UMMC_Base()
{
	StrengthDef.AttributeToCapture = UBrandNewAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	StrengthDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(StrengthDef);

	IntelligenceDef.AttributeToCapture = UBrandNewAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	IntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(IntelligenceDef);

	DexterityDef.AttributeToCapture = UBrandNewAttributeSet::GetDexterityAttribute();
	DexterityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DexterityDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(DexterityDef);

	VitalityDef.AttributeToCapture = UBrandNewAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	VitalityDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(VitalityDef);
	
}
