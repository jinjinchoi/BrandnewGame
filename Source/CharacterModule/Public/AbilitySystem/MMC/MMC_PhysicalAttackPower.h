// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMC_Base.h"
#include "MMC_PhysicalAttackPower.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UMMC_PhysicalAttackPower : public UMMC_Base
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
	
};
