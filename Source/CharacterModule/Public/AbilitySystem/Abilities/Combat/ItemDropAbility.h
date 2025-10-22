// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ItemDropAbility.generated.h"

class ABrandNewPickupItem;

USTRUCT(BlueprintType)
struct FDropItemParams
{
	GENERATED_BODY()

	/* 드랍할 아이템의 아이디 */
	UPROPERTY(EditDefaultsOnly)
	int32 ItemId = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 Quantity = 0.f;

	/* 드랍할 확률 가중치 */
	UPROPERTY(EditDefaultsOnly)
	float Weight = 1.f;

	/* 아이템 드랍 갯수 편차 */
	UPROPERTY(EditDefaultsOnly)
	int32 RandomDeviation = 0;
	
};

/**
 * 에너미가 죽을 때 아이템을 확률적으로 드랍하는 어빌리티
 */
UCLASS()
class CHARACTERMODULE_API UItemDropAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Item")
	void DropItem();
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Item")
	TArray<FDropItemParams> DropItems;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Item", meta=(MustImplement = "PickupItemInterface"))
	TSubclassOf<AActor> ItemClass;

private:
	FDropItemParams GetItemPram();
	
};
