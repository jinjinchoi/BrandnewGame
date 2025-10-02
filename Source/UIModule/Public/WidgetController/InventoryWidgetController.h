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

	/**
	 * 소비템을 사용하는 함수. 슬롯 인덱스가 곧 배열의 인덱스이며 해당 배열에 있는 아이템을 사용함.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Inventory")
	void UseEatableItem(const int32 SlotIndex) const;

public:
	FORCEINLINE void SetPlayerState(APlayerState* PS) { PlayerState = PS; }
	
};
