// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataTableStruct/DataTableParams.h"
#include "UObject/Object.h"
#include "BrandNewInventory.generated.h"

/* 슬롯 하나에 담겨 있는 아이템 정보 */
USTRUCT(BlueprintType)
struct FInventorySlotData
{
	GENERATED_BODY()

	/* 아이디를 저장하여 TMap에서 아이템 정보를 가져옴 */
	UPROPERTY(BlueprintReadOnly)
	int32 ItemID;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Quantity = 0;
	
};

/* 전체 인벤토리 */
USTRUCT(BlueprintType)
struct FInventoryContents
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlotData> WeaponSlots;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlotData> ArmorSlots;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlotData> EatablesSlots;
	
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class INVENTORYMODULE_API UBrandNewInventory : public UObject
{
	GENERATED_BODY()

public:
	void Init();
	void AddItemToSlot(const FInventorySlotData& NewItem);

protected:
	FInventoryContents ItemInventory;

	/* 아이템의 정보를 담고 있는 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> ItemDataTable;

private:
	void StackItemIntoInventory(const FInventorySlotData& NewItem);
	
	/* 아이템의 아이디를 기반으로 아이템 정보를 저장하는 TMap */
	TMap<int32, FItemDataRow> ItemIDMap;
	
	
};
