// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"
#include "InventoryModule/Public/Inventory/BrandNewInventory.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	Inventory = CreateDefaultSubobject<UBrandNewInventory>(TEXT("Inventory"));
	
}


UBrandNewInventory* ABrandNewPlayerState::GetInventoryInterfaceClass() const
{
	return Inventory;
}

