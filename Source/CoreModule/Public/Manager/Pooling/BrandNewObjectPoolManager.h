// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BrandNewObjectPoolManager.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Classes", meta = (MustImplement = "PoolableActorInterface"))
	TArray<TSubclassOf<AActor>> ObjectClasses;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Object")
	int32 PoolSizePerType = 5;

private:
	void CreatePool(const TArray<TSubclassOf<AActor>>& InObjectClasses);
	
	/* 오브젝트를 저장하는 Tap으로 클래스와 액터를 매칭하여 저장. */
	TMap<UClass*, TArray<AActor*>> ObjectPools;
	
};
