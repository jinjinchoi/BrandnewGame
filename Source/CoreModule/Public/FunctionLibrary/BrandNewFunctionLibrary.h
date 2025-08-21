// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrandNewFunctionLibrary.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
struct FBaseAttributePrams;
/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandNewFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ApplyPrimaryAttributesSetByCaller(
		const FBaseAttributePrams& InAttributePrams, UAbilitySystemComponent* InASC, const TSubclassOf<UGameplayEffect>& EffectToApply);
	
};
