// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_AttributeInfo.h"

FGameplayAttribute UDataAsset_AttributeInfo::FindAttributeGetter(const FGameplayTag& InAttributeTag) const
{
	for (const FBrandNewAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTag(InAttributeTag))
		{
			return Info.AttributeGetter;
		}
	}
	return FGameplayAttribute();
}
