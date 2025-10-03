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

	ItemStrengthDef.AttributeToCapture = UBrandNewAttributeSet::GetItemStrengthAttribute();
	ItemStrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ItemStrengthDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ItemStrengthDef);

	ItemIntelligenceDef.AttributeToCapture = UBrandNewAttributeSet::GetItemIntelligenceAttribute();
	ItemIntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ItemIntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ItemIntelligenceDef);

	ItemDexterityDef.AttributeToCapture = UBrandNewAttributeSet::GetItemDexterityAttribute();
	ItemDexterityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ItemDexterityDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ItemDexterityDef);

	ItemVitalityDef.AttributeToCapture = UBrandNewAttributeSet::GetItemVitalityAttribute();
	ItemVitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	ItemVitalityDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(ItemVitalityDef);

	
}
