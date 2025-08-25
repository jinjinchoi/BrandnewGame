// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrandNewFunctionLibrary.generated.h"

struct FGameplayTag;
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

	UFUNCTION(BlueprintCallable, Category = "BrandNewFunctionLibrary|Gameplay Tag")
	static bool DoseActorHasTag(AActor* TargetActor, const FGameplayTag& TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "BrandNewFunctionLibrary|Gameplay Tag")
	static void AddGameplayTagToActor(AActor* TargetActor, const FGameplayTag& TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "BrandNewFunctionLibrary|Gameplay Tag")
	static void RemoveGameplayTagToActor(AActor* TargetActor, const FGameplayTag& TagToAdd);
	
};
