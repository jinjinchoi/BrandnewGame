// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "CharacterInfoWidgetController.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class UIMODULE_API UCharacterInfoWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "BrandNew|Attribute")
	float GetAttributeByTag(const FGameplayTag& AttributeTag) const;
	
};
