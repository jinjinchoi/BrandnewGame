// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MMC/MMC_Base.h"
#include "MMC_HealthRegeneration.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UMMC_HealthRegeneration : public UMMC_Base
{
	GENERATED_BODY()
	
public:
	UMMC_HealthRegeneration();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition MaxHealthDef;
	
	
};
