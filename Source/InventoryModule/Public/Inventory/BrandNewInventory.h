// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Interfaces/InventorySystem/BrandNewInventoryInterface.h"
#include "UObject/Object.h"
#include "BrandNewInventory.generated.h"

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

	/*
	 * 아이템을 소비하는 함수. 여기서 소비란 배열에서 아이템을 제거하는 것을 말함.
	 * 즉, 아이템 삭제용으로도 이 함수를 사용하면 됨.
	 */
	void ConsumeItemAtSlot(const EItemType ItemType, const int32 SlotIndex, const int32 NumOfRemoval = 1);

protected:
	UPROPERTY(Replicated)
	FInventoryContents ItemInventory;

	/* 아이템의 정보를 담고 있는 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> ItemDataTable;

private:
	void StackItemIntoInventory(const FInventorySlotData& NewItem);
	
public:
	FORCEINLINE FInventoryContents GetInventory() const { return ItemInventory; }
	
};
