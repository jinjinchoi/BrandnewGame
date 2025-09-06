// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributeChanged, float, NewValue);

class UBrandNewAttributeSet;
class UBrandNewAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class UIMODULE_API UOverlayWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies(APawn* InControlledPawn) const override;
	virtual void BroadCastInitialValue() override;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnMaxManaChangedDelegate;
	
};
