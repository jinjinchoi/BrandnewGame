// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/ItemDropAbility.h"

#include "CharacterFunctionLibrary.h"
#include "DebugHelper.h"
#include "Interfaces/Actor/PickupItemInterface.h"

void UItemDropAbility::DropItem()
{
	if (!ItemClass) return;

	const FDropItemParams ItemParams = GetItemPram();
	if (ItemParams.ItemId <= 0)	return; // 아이디 0번 이하로 설정되어있으면 생성 방지. 이걸로 드랍 확률 설정.
	
	if (ItemParams.Quantity <= 0)
	{
		const FString WarningMessage = FString::Printf(TEXT("Item Quantity is 0 or negative! Class: %s"),  *GetClass()->GetName());
		DebugHelper::Print(WarningMessage, FColor::Red);
		return;
	}
	
	
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	FVector SpawnLocation = FVector::ZeroVector;
	UCharacterFunctionLibrary::GetValidGroundLocation(
		GetOwningActorFromActorInfo(), GetOwningActorFromActorInfo()->GetActorLocation(), SpawnLocation, 50);

	// 트랜스폼 생성
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	if (AActor* PickupItemActor = GetWorld()->SpawnActorDeferred<AActor>(ItemClass, SpawnTransform))
	{
		IPickupItemInterface* PickupItemInterface = CastChecked<IPickupItemInterface>(PickupItemActor);

		const int32 Quantity = ItemParams.Quantity + FMath::RandRange(0, ItemParams.RandomDeviation);
		
		PickupItemInterface->SetId(ItemParams.ItemId);
		PickupItemInterface->SetQuantity(Quantity);

		PickupItemActor->FinishSpawning(SpawnTransform);
	}
	
}

FDropItemParams UItemDropAbility::GetItemPram()
{
	float TotalWeight = 0.0f;

	for (const FDropItemParams& ItemData : DropItems)
	{
		TotalWeight += ItemData.Weight;
	}

	const float RandomValue = FMath::RandRange(0.0f, TotalWeight);
	float AccumulatedWeight = 0.0f;

	for (const FDropItemParams& ItemData : DropItems)
	{
		AccumulatedWeight += ItemData.Weight;
		if (AccumulatedWeight >= RandomValue)
		{
			return ItemData;
		}
	}

	return DropItems.Last();
	
}
