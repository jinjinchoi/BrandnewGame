// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"
#include "InventoryModule/Public/Inventory/BrandNewInventory.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	Inventory = CreateDefaultSubobject<UBrandNewInventory>(TEXT("Inventory"));
	
}

FInventoryContents ABrandNewPlayerState::GetInventoryContents() const
{
	return Inventory->GetInventory();
}


UBrandNewInventory* ABrandNewPlayerState::GetInventory() const
{
	return Inventory;
}

