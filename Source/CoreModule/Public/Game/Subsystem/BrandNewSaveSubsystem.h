// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BrandNewSaveSubsystem.generated.h"


/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandNewSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static void SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams);
	static FSaveSlotPrams GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex);

	FString SlotToLoad;
	int32 SlotIndexToLoad;
	
};
