// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstance/BrandNewGameInstance.h"

#include "DataAssets/DataAsset_MapInfo.h"

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
