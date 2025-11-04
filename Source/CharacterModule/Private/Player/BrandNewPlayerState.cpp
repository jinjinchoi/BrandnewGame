// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"

#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "InventoryModule/Public/Inventory/BrandNewInventory.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	Inventory = CreateDefaultSubobject<UBrandNewInventory>(TEXT("Inventory"));
	
}

FInventoryContents ABrandNewPlayerState::GetInventoryContents() const
{
	if (Inventory)
	{
		return Inventory->GetInventoryContents();
	}

	return FInventoryContents();
}


UBrandNewInventory* ABrandNewPlayerState::GetInventory() const
{
	if (IsValid(Inventory))
	{
		return Inventory;
	}
	return nullptr;
}
