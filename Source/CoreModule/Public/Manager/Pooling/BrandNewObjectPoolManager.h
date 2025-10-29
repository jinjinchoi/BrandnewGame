// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BrandNewObjectPoolManager.generated.h"

USTRUCT(BlueprintType)
struct FLevelPoolData
{
	GENERATED_BODY()
	
	/* 레벨 별로 스폰할 액터를 설정하기 위해서 레벨 정도 세팅 */
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LevelWorld;
	
	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "PoolableActorInterface"))
	TArray<TSubclassOf<AActor>> ObjectClasses;
};

/**
 * 
 */
UCLASS(Blueprintable)
class COREMODULE_API UBrandNewObjectPoolManager : public UObject
{
	GENERATED_BODY()

public:
	void InitPoolManager();
	
	/* 클래스를 기반으로 풀에 저장된 오브젝트를 찾아오는 함수. 풀에 오브젝트가 존재하지 않으면 새로 만들어서 반환 */
	AActor* GetPooledObject(const TSubclassOf<AActor>& ActorClass);
	
	/* 오브젝트 풀에 액터를 돌려 넣는 함수 */
	void ReturnObject(AActor* ActorToReturn);


protected:
	/* 모든 Level에서 Pool에 생성해야할 Object Type */
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Classes", meta = (MustImplement = "PoolableActorInterface"))
	TArray<TSubclassOf<AActor>> SharedObjectClasses;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Classes")
	TArray<FLevelPoolData> SpawnDataPrams;

	// 하나의 오브젝트 당 스폰할 갯수
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Parameters")
	int32 PoolSizePerType = 5;

	// 한 프레임당 스폰할 횟수
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Parameters")
	int32 SpawnLimitPerFrame = 10; 

private:
	/* 오브젝트를 저장하는 Map으로 클래스와 액터를 매칭하여 저장. */
	TMap<UClass*, TArray<AActor*>> ObjectPools;

	// 현재 Level에서 스폰해야할 클래스 정보를 가져오는 함수
	TArray<TSubclassOf<AActor>> GetCurrentLevelSpawnActorClasses() const;
	void TimerTick();
	// 프레임 단위로 제한 수만큼 오브젝트 생성하는 함수
	void SpawnStep();
	
	/**
	 * 현재 풀에 등록중인 오브젝트가 총 스폰된 횟수로
	 * 프레임당 스폰이 되다가 다음 프레임으로 넘어가기 때문에 그전 프레임에 얼마나 스폰했는지를 저장
	 */
	int32 CurrentSpawnCount = 0;
	int32 CurrentClassIndex = 0;
	bool bInitComplete = false;
	FTimerHandle SpawnTimerHandle;

	/* 현재 스폰할 액터의 정보 Array */
	UPROPERTY()
	TArray<TSubclassOf<AActor>> TargetObjectClasses;

	bool IsSameLevel(const TSoftObjectPtr<UWorld> TargetWorld) const;
	
};
