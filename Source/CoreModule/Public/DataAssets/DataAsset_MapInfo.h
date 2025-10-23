// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_MapInfo.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MapAsset = nullptr; // 실제 월드 에셋

	UPROPERTY(EditDefaultsOnly)
	FText DisplayName = FText(); // 표시할 이름
	
};

/**
 * 
 */
UCLASS()
class COREMODULE_API UDataAsset_MapInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FMapInfo> MapInfos;

	FText GetDisplayName(const TSoftObjectPtr<UWorld> MapAsset) const;
	
};
