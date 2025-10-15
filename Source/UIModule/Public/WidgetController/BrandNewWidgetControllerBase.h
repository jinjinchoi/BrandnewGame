// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UIMODULE_API UBrandNewWidgetControllerBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() const;

	UFUNCTION(BlueprintCallable, Category = "BrandNew|WidgetControllerFunc")
	virtual void BroadCastInitialValue();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BrandNew|Character")
	TObjectPtr<APawn> ControlledPawn;

public:
	FORCEINLINE void SetControlledPawn(APawn* PawnToSet) { ControlledPawn = PawnToSet; };
	
};
