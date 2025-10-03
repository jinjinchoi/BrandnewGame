// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Base.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UMMC_Base : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_Base();

protected:
	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
	FGameplayEffectAttributeCaptureDefinition DexterityDef;
	FGameplayEffectAttributeCaptureDefinition VitalityDef;

	//TODO: Item Attribute 추가해야함
	FGameplayEffectAttributeCaptureDefinition ItemStrengthDef;
	FGameplayEffectAttributeCaptureDefinition ItemIntelligenceDef;
	FGameplayEffectAttributeCaptureDefinition ItemDexterityDef;
	FGameplayEffectAttributeCaptureDefinition ItemVitalityDef;
	
};
