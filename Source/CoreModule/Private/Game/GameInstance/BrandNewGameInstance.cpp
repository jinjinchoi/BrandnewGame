// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstance/BrandNewGameInstance.h"
#include "DataAssets/DataAsset_MapInfo.h"
#include "Manager/Sequnce/SequenceManager.h"

FText UBrandNewGameInstance::GetMapName(const FString& MapPath) const
{
	for (const FMapInfo& MapInfo : MapInfoDataAsset->MapInfos)
	{
		const FSoftObjectPath& SoftObjectPath = MapInfo.MapAsset.ToSoftObjectPath();
		const FString PackageName = SoftObjectPath.GetLongPackageName();
		if (PackageName == MapPath)
		{
			return MapInfo.DisplayName;
		}
	}

	return FText::GetEmpty();
	
}

FText UBrandNewGameInstance::GetMapName(const TSoftObjectPtr<UWorld> SoftMapClass) const
{
	return MapInfoDataAsset->GetDisplayName(SoftMapClass);
	
}

FItemDataRow UBrandNewGameInstance::GetItemData(const int32 ItemId)
{
	if (ItemIdMap.IsEmpty()) return FItemDataRow();

	if (ItemIdMap.Contains(ItemId))
	{
		return *ItemIdMap.Find(ItemId);
	}

	return FItemDataRow();
	
}

void UBrandNewGameInstance::Init()
{
	Super::Init();

	check(ItemDataTable && SequenceManagerClass)
	if (!ItemDataTable || !SequenceManagerClass) return;

	// 데이터 테이블의 Row 구조체가 설정한 구조체와 맞는지 확인
	if (ItemDataTable->GetRowStruct() == FItemDataRow::StaticStruct())
	{
		for (const TPair<FName, unsigned char*>& RowMap : ItemDataTable->GetRowMap()) // 데이터 테이블을 순회하여 아이디와 아이템 정보를 저장
		{
			const FItemDataRow* ItemData = reinterpret_cast<FItemDataRow*>(RowMap.Value);
			ItemIdMap.Add(ItemData->ID, *ItemData);
		}
	}

	// 시퀀스 매니저 생성
	SequenceManager = NewObject<USequenceManager>(this, SequenceManagerClass);
	
}
