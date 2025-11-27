// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"

#include "Components/BrandnewQuestComponent.h"
#include "InventoryModule/Public/Inventory/BrandNewInventory.h"
#include "Net/UnrealNetwork.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	Inventory = CreateDefaultSubobject<UBrandNewInventory>(TEXT("Inventory"));
	QuestComponent = CreateDefaultSubobject<UBrandnewQuestComponent>(TEXT("QuestComponent"));
	
}

void ABrandNewPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerUniqueId);
	
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

UActorComponent* ABrandNewPlayerState::GetQuestComponent() const
{
	if (IsValid(QuestComponent))
	{
		return QuestComponent;
	}
	
	return nullptr;
}

void ABrandNewPlayerState::SetPlayerUniqueId(const FString& InPlayerId)
{
	PlayerUniqueId = InPlayerId;
	OnPlayerIdSetDelegate.Broadcast(PlayerUniqueId);
}


UBrandNewInventory* ABrandNewPlayerState::GetInventory() const
{
	if (IsValid(Inventory))
	{
		return Inventory;
	}
	return nullptr;
}

void ABrandNewPlayerState::GrantQuestByLevelRequirement(const int32 PlayerLevel) const
{
	check(QuestComponent);
	if (!QuestComponent) return;
	
	QuestComponent->GrantQuestByLevelRequirement(PlayerLevel);
	
}

void ABrandNewPlayerState::OnRep_PlayerUniqueId()
{
	OnPlayerIdSetDelegate.Broadcast(PlayerUniqueId);
}


