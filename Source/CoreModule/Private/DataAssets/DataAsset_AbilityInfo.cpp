// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_AbilityInfo.h"

FAbilityInfoParams UDataAsset_AbilityInfo::FindAbilityInfoByAbilityTag(const FGameplayTag& AbilityTagToFind)
{
	for (const FAbilityInfoParams& AbilityInfoParams : AbilityInformation)
	{
		if (AbilityInfoParams.AbilityTag == AbilityTagToFind)
		{
			return AbilityInfoParams;
		}
	}

	return FAbilityInfoParams();
}
