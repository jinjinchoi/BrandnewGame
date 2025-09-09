// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_LevelUpInfo.generated.h"

USTRUCT(BLueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 5;
	
};

/**
 * 
 */
UCLASS()
class COREMODULE_API UDataAsset_LevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="LevelUpInfo")
	TArray<FLevelUpInfo> LevelUpInformation;

	/* XP를 보내면 그에 맞는 레벨을 반환하는 함수 */
	int32 FindLevelForXP(const int32 XP) const;
	
};
