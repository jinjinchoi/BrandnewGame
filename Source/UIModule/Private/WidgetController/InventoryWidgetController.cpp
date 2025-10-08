// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/InventoryWidgetController.h"

#include "GameFramework/PlayerState.h"
#include "Interfaces/Animation/BrandNewPlayerAnimInterface.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"

FInventoryContents UInventoryWidgetController::GetInventoryContents() const
{
	check(PlayerState)

	if (const IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(PlayerState))
	{
		return PlayerStateInterface->GetInventoryContents();
	}

	return FInventoryContents();
	
}

void UInventoryWidgetController::UseEatableItem(const int32 SlotIndex) const
{
	if (SlotIndex < 0) return;
	
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn))
	{
		PlayerInterface->UseConsumptionItem(SlotIndex);
	}
}

void UInventoryWidgetController::EquipItem(const int32 SlotIndex, const EItemType ItemType) const
{
	if (SlotIndex < 0) return;
	
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn))
	{
		PlayerInterface->UseEquipmentItem(SlotIndex, ItemType);
	}
}