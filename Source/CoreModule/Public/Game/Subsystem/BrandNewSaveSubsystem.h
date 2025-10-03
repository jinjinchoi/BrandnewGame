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
	void SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams);
	
	/* 슬롯에서 데이터를 가져오는 함수 */
	FSaveSlotPrams GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex);
	
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

	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	void Login(const FString& Id);

	UFUNCTION(BlueprintPure, Category = "Brandnew|Save Logic")
	FString GetUniqueIdentifier() const;

private:
	/* 캐릭터의 데이터가 저장되어있는 구조체 */
	FSaveSlotPrams LastestPlayerData;

	/* 세이브 로드시 슬롯 이름 앞에 붙여 고유 저장소를 만들때 사용
	 * 현재 가장 큰 목적은 클라이언트가 서버에 저장할때 리슨서버이기 때문에 서버의 저장파일도 존재하며,
	 * 그렇기 때문에 이와 구분하기 위하여 고유 식별자를 사용.
	 */
	FString UniqueIdentifier;
	
};
