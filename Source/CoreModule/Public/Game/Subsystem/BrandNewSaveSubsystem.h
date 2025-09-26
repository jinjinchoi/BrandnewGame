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
	/* 슬롯에 데이터를 저장하는 함수 */
	static void SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams);
	
	/* 슬롯에서 데이터를 가져오는 함수 */
	static FSaveSlotPrams GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex);
	
	/* 서브시스템에 데이터를 저장하는 함수. */
	void SavePlayerData(const FSaveSlotPrams& SaveSlotPrams);
	
	/* 서브시스템에 데이터를 가져오는 함수. */
	FSaveSlotPrams GetLastestPlayerData();
	
	/* 서브시스템에 있는 데이터를 초기화하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	void ResetPlayerData();

	/*
	 * 슬롯에서 세이브 데이터를 가져와 서브시스템에 저장하도록 요청하는 함수
	 * return: 세이브 데이터를 정상적으로 가져와 저장하였으면 맵 에셋 네임을 반환.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	FString RequestCharacterDataLoad(const FString& SlotName, const int32 SlotIndex);


private:
	/* 캐릭터의 데이터가 저장되어있는 구조체 */
	FSaveSlotPrams LastestPlayerData;
	
};
