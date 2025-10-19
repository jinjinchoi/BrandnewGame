// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_MapInfo.h"

FText UDataAsset_MapInfo::GetDisplayName(const TSoftObjectPtr<UWorld> MapAsset) const
{
	for (const FMapInfo& Data : MapInfos)
	{
		if (Data.MapAsset.ToSoftObjectPath() == MapAsset.ToSoftObjectPath())
		{
			return FText::FromStringTable("ST_MapNames", Data.DisplayNameKey.ToString());
		}
	}

	return FText::GetEmpty();
	
}
