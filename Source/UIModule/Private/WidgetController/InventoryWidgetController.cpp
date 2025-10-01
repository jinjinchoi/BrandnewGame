// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/InventoryWidgetController.h"

#include "GameFramework/PlayerState.h"
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
