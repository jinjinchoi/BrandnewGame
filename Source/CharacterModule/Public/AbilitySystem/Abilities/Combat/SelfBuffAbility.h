// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BandNewBaseGameplayAbility.h"
#include "SelfBuffAbility.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API USelfBuffAbility : public UBandNewBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Brandnew|GameplayAbility")
	void ApplyBuffEffectToSelf() const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Effect")
	TSubclassOf<UGameplayEffect> BuffEffectClass;
	
};
