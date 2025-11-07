// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"

#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "InventoryModule/Public/Inventory/BrandNewInventory.h"
#include "Net/UnrealNetwork.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	Inventory = CreateDefaultSubobject<UBrandNewInventory>(TEXT("Inventory"));
	
}

void ABrandNewPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerUniqueId);
	
}

void ABrandNewPlayerState::BeginPlay()
{
	Super::BeginPlay();

	const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	check(SaveSubsystem);

	if (HasAuthority())
	{
		PlayerUniqueId = SaveSubsystem->GetUniqueIdentifier();
	}
	else
	{
		Server_SetPlayerUniqueId(SaveSubsystem->GetUniqueIdentifier());
	}
	
}

void ABrandNewPlayerState::Server_SetPlayerUniqueId_Implementation(const FString& NewPlayerUniqueId)
{
	PlayerUniqueId = NewPlayerUniqueId;
	
}

FInventoryContents ABrandNewPlayerState::GetInventoryContents() const
{
	if (Inventory)
	{
		return Inventory->GetInventoryContents();
	}

	return FInventoryContents();
}

FString ABrandNewPlayerState::GetPlayerUniqueId() const
{
	return PlayerUniqueId;
}


UBrandNewInventory* ABrandNewPlayerState::GetInventory() const
{
	if (IsValid(Inventory))
	{
		return Inventory;
	}
	return nullptr;
}


