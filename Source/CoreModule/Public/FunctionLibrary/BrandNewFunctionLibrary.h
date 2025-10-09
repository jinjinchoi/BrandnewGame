// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrandNewFunctionLibrary.generated.h"

class UBrandNewObjectPoolManager;
/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandNewFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UBrandNewObjectPoolManager* GetObjectPoolManager(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="BrandNewFunctionLibrary|InventorySystem", meta=(WorldContext="WorldContextObject"))
	static FItemDataRow GetItemData(const UObject* WorldContextObject, const int32 ItemId);

	UFUNCTION(BlueprintPure, Category = "BrandNewFunctionLibrary|String")
	static bool IsOnlyEnglishWord(const FString& InputWord);

	UFUNCTION(BlueprintPure, Category = "BrandNewFunctionLibrary|String")
	static bool IsValidIPAddressWord(const FString& Address);
	
};
