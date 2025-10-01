// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "InventoryWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class UIMODULE_API UInventoryWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState;

	UFUNCTION(BlueprintPure, Category = "Brandnew|Inventory")
	FInventoryContents GetInventoryContents() const;

public:
	FORCEINLINE void SetPlayerState(APlayerState* PS) { PlayerState = PS; }
	
};
