// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "GameFramework/SaveGame.h"
#include "BrandNewSlotSaveGame.generated.h"



/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandNewSlotSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FAttributeSaveData AttributePrams;

	UPROPERTY()
	FVector CharacterLocation;

	UPROPERTY()
	FText CurrentMapName;

	UPROPERTY()
	FText TitleText;

	UPROPERTY()
	FString MapPackageName;

	UPROPERTY()
	TMap<FGameplayTag /* 어빌리티 태그 */, int32 /* 어빌티리 레빌 */> AbilityDataMap;

	UPROPERTY()
	FText SavedTime;

	UPROPERTY()
	FInventoryContents Inventory;
	
};
