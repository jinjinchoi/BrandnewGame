// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FORCEINLINE int32 GetTestNumber() const { return TestNumber; }
	FORCEINLINE void AddTestNumber() { TestNumber++; }
	FORCEINLINE FInventoryContents GetInventory() const { return ItemInventory; }
	
};
