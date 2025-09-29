// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "Engine/GameInstance.h"
#include "BrandNewGameInstance.generated.h"

class UDataAsset_MapInfo;
/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandNewGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FText GetMapName(const FString& MapPath) const;
	FItemDataRow GetItemData(const int32 ItemId);

protected:
	virtual void Init() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataAsset")
	TObjectPtr<UDataAsset_MapInfo> MapInfoDataAsset;

	/* 아이템의 정보를 담고 있는 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> ItemDataTable;

private:
	/* 아이템의 아이디를 기반으로 아이템 정보를 저장하는 TMap */
	TMap<int32, FItemDataRow> ItemIdMap;

	
	
};
