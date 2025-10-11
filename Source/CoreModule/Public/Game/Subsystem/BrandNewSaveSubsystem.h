// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BrandNewSaveSubsystem.generated.h"


/**
 * 세이브 로드 작업을 하는 서브시스템.
 * 세이브 로드가 아이디 기반과 아이디 매개변수 없이 멤버 변수 UniqueIdentifier를 사용하는 함수로 나누어져 있는데
 * 서버에서 클라이언트의 서브시스템에 접근할 수 없기 때문에 클라이언트에서 세이브시 클라이언트의 아이디를 얻어서 이를 서버에 보내야해서 세이브 로드 로직을 두가지 버전으로 나눔.
 */
UCLASS()
class COREMODULE_API UBrandNewSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	/* 슬롯에 데이터를 저장하는 함수 */
	void SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams) const;
	static void SaveGameToSlotWithId(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams, const FString& UniqueId);

	FSaveSlotPrams GetSaveDataById(const FString& SlotName, const int32 SlotIndex, const FString& UniqueId) const;
	
	/* 슬롯에서 데이터를 가져오는 함수 */
	FSaveSlotPrams GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex) const;

	/* 본 클래스에 존재하는 CurrentSlotName의 슬롯에서 데이터를 반환하는 함수 */
	FSaveSlotPrams GetSaveDataInCurrentSlot() const;
	
	FSaveSlotPrams GetCurrentSlotSaveDataById(const FString& UniqueId) const;
	
	/* 서브시스템에 있는 데이터를 초기화하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	void ResetPlayerData();

	/**
	 * 맵 에셋 네임을 반환하는 함수.
	 * 맵을 이동하기전에 사용하는 함수이기 때문에 에셋 네임을 성공적으로 반환하면 슬롯 데이터를 저장까지 진행.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	FString TryGetMapAssetNameAndSaveSlotInfo(const FString& SlotName, const int32 SlotIndex);
	
	void SetCurrentSlotNameAndIndex(const FString& SlotName, const int32 SlotIndex);
	
	/* 플레이어의 UniqueIdentifier를 저장할때 사용하는 함수  */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	void Login(const FString& Id);

	UFUNCTION(BlueprintPure, Category = "Brandnew|Save Logic")
	FString GetUniqueIdentifier() const;

private:
	/**
	 * 세이브 로드시 슬롯 이름 앞에 붙여 고유 저장소를 만들때 사용.
	 * 현재 가장 큰 목적은 클라이언트별로 세이브 데이터 슬롯을 구분하기 위하여 고유 식별자를 사용.
	 */
	FString UniqueIdentifier;

	/* 서버가 로드할때 사용한 슬롯의 정보를 담는 변수. */
	FString CurrentSlotName = FString();
	int32 CurrentSlotIndex = 0;
	bool bIsLoadedWorld = false;

public:
	FORCEINLINE bool IsLoadedWorld() const { return bIsLoadedWorld; }
	
};
