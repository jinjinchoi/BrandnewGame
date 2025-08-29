// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributeChanged, float, NewValue);

class UBrandNewAttributeSet;
class UBrandNewAbilitySystemComponent;
/**
 * 
 */
UCLASS(BlueprintType)
class UIMODULE_API UOverlayWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void BindCallbacksToDependencies(APawn* InControlledPawn) const;

	UFUNCTION(BlueprintCallable, Category = "BrandNew|WidgetControllerFunc")
	void BroadCastInitialValue();

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnPlayerAttributeChanged OnMaxManaChangedDelegate;

private:
	UPROPERTY()
	TObjectPtr<APawn> ControlledPawn;

public:
	FORCEINLINE void SetControlledPawn(APawn* PawnToSet) { ControlledPawn = PawnToSet; };
	
};
