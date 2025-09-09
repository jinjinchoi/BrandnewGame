// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_LevelUpInfo.h"

int32 UDataAsset_LevelUpInfo::FindLevelForXP(const int32 XP) const
{
	int32 Level = 0;
	
	while (Level + 1 < LevelUpInformation.Num() && XP >= LevelUpInformation[Level + 1].LevelUpRequirement)
	{
		++Level;
	}

	return Level;
}
