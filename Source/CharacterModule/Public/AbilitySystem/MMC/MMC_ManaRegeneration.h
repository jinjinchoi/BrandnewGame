// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MMC/MMC_Base.h"
#include "MMC_ManaRegeneration.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UMMC_ManaRegeneration : public UMMC_Base
{
	GENERATED_BODY()
	
public:
	UMMC_ManaRegeneration();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition MaxManaDef;
	
	
};
