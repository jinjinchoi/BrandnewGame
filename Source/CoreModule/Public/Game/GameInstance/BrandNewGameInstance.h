// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataAsset")
	TObjectPtr<UDataAsset_MapInfo> MapInfoDataAsset;
	
};
