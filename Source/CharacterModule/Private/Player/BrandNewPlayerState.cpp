// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"
#include "Net/UnrealNetwork.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
}

void ABrandNewPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, Inventory, COND_OwnerOnly);
	
}

void ABrandNewPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	SetupInventory();
	
}

void ABrandNewPlayerState::SetupInventory()
{
	check(InventoryClass);
	if (Inventory || !HasAuthority() || !InventoryClass) return;
	
	Inventory = NewObject<UObject>(this, InventoryClass);
	
}
