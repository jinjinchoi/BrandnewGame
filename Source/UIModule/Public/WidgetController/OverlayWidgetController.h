// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OverlayWidgetController.generated.h"

class UBrandNewAttributeSet;
class UBrandNewAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UIMODULE_API UOverlayWidgetController : public UObject
{
	GENERATED_BODY()

public:
	

	void BindCallbacksToDependencies();

private:
	UPROPERTY()
	TObjectPtr<UBrandNewAbilitySystemComponent> BnAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBrandNewAttributeSet> BnAttributeSet;
	
};
