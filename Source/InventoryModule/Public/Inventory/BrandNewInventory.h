// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/InventorySystem/BrandNewInventoryInterface.h"
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
UCLASS()
class INVENTORYMODULE_API UBrandNewInventory : public UActorComponent, public IBrandNewInventoryInterface
{
	GENERATED_BODY()

public:
	UBrandNewInventory();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void AddItemToSlot(const FInventorySlotData& NewItem) override;

protected:
	UPROPERTY(Replicated)
	FInventoryContents ItemInventory;

	/* 아이템의 정보를 담고 있는 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Replicated)
	int32 TestNumber = 0;

private:
	void StackItemIntoInventory(const FInventorySlotData& NewItem);
	
public:
	int32 GetTestNumber() const { return TestNumber; }
	void AddTestNumber() { TestNumber++; }
	
};
