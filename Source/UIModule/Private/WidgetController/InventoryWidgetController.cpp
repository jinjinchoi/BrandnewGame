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
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn))
	{
		PlayerInterface->UseConsumptionItem(SlotIndex);
	}
}
