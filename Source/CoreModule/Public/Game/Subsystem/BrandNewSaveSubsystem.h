// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BrandNewSaveSubsystem.generated.h"


/**
 * 세이브 로드 작업을 하는 서브시스템.
 * 세이브 슬롯은 UniqueIdentifier로 구분 되는데 SubSystem에서 직접 클라이언트의 UniqueIdentifier에 접근할 수 없기 때문에
 * 클라이언트의 데이터를 저장하거나 로드하기 위해서는 UniqueIdentifier를 매개변수로 보내야함.
 * 세이브 로드를 서버에서만 하기 때문에 서버는 굳이 안보내도 되나 함수의 통일을 위해 서버(호스트)도 동일한 로직을 사용.
 */
UCLASS()
class COREMODULE_API UBrandNewSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/* 슬롯에 데이터를 저장하는 함수 */
	static void SaveGameToSlotWithId(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams, const FString& UniqueId);

	FSaveSlotPrams GetSaveDataById(const FString& SlotName, const int32 SlotIndex, const FString& UniqueId) const;

	/* 현재 슬롯은 호스트에서 로드하였을때 클라이언트가 호스트의 로드 슬롯으로 데이터를 로드하기 위해서 사용. */
	FSaveSlotPrams GetCurrentSlotSaveDataById(const FString& UniqueId) const;
	
	/* 서브시스템에 있는 데이터를 초기화하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	void ResetPlayerData();

	/* 맵 에셋 네임을 반환하는 함수.
	 * 맵을 이동하기전에 사용하는 함수이기 때문에 에셋 네임을 성공적으로 반환하면 슬롯 데이터를 저장까지 진행. */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	FString TryGetMapAssetNameAndSaveSlotInfo(const FString& SlotName, const int32 SlotIndex);
	
	void SetCurrentSlotNameAndIndex(const FString& SlotName, const int32 SlotIndex);
	
	/* 플레이어의 UniqueIdentifier를 저장할때 사용하는 함수  */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Save Logic")
	void Login(const FString& Id);

	UFUNCTION(BlueprintPure, Category = "Brandnew|Save Logic")
	FString GetUniqueIdentifier() const;
	
	void UpdateLatestPlayerDataMap(const FString& PlayerName, const FSaveSlotPrams& SaveSlotPrams);
	
	/* 맵 이동 후 기존 데이터를 가져오는 함수 */
	FSaveSlotPrams GetLatestPlayerData(const FString& PlayerName) const;
	
	/**
	 * LatestPlayerDataMap에서 특정 플레이어의 정보를 삭제하는 작업.
	 * 현재 캐릭터의 연결이 종료될때 해당 함수를 호출하도록 구현하였음.
	 */
	void RemoveLatestPlayerData(const FString& PlayerName);

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

	/**
	 *  LatestPlayerData란 맵 이동 직전 플레이어가 가지고 있던 데이터를 말함.
	 * 로드시 이 변수의 유효성을 확인하여 내부 데이터가 있으면 맵 이동으로 판단하여
	 * 로드 로직을 해당 맵에서 데이터를 가져와 하는 것으로 진행.
	 */
	TMap<FString, FSaveSlotPrams> LatestPlayerDataMap;
	

public:
	FORCEINLINE bool IsLoadedWorld() const { return bIsLoadedWorld; }
	FORCEINLINE void ClearLatestPlayerDataMap() { LatestPlayerDataMap.Empty(); }
};
