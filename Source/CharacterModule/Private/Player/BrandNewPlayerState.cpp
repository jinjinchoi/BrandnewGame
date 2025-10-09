// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerState.h"

#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "InventoryModule/Public/Inventory/BrandNewInventory.h"

ABrandNewPlayerState::ABrandNewPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	Inventory = CreateDefaultSubobject<UBrandNewInventory>(TEXT("Inventory"));
	
}

void ABrandNewPlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();

	// TODO: 실제 패키징 후에도 복제 시점 유효한지 확인해야함.
	bIsPlayerNameSet = true;
	NotifyWhenPawnReady();
	
}

FInventoryContents ABrandNewPlayerState::GetInventoryContents() const
{
	return Inventory->GetInventoryContents();
}


UBrandNewInventory* ABrandNewPlayerState::GetInventory() const
{
	return Inventory;
}



FOnPlayerSetDelegate& ABrandNewPlayerState::GetPlayerSetDelegate()
{
	return OnPlayerSetDelegate;
}

void ABrandNewPlayerState::SetPlayerNameToPlayerState(const FString& NewName)
{

	if (HasAuthority())
	{
		Super::SetPlayerName(NewName);
		
		bIsPlayerNameSet = true;
		NotifyWhenPawnReady();
	}
	else
	{
		Server_SetPlayerName(NewName);
		bIsPlayerNameSet = true;
	}

	
}

bool ABrandNewPlayerState::IsPlayerReplicated() const
{
	return bIsPawnSet && bIsPlayerNameSet;

}

void ABrandNewPlayerState::NotifyWhenPawnReady()
{
	if (GetPawn())
	{
		bIsPawnSet = true;
		OnPlayerSetDelegate.ExecuteIfBound();
	}
	else
	{
		TWeakObjectPtr<APlayerState> WeakPS = this;
		if (WeakPS.IsValid())
		{
			WeakPS->OnPawnSet.AddUniqueDynamic(this, &ThisClass::OnPlayerPawnPossessed);
		}
	}
	
}

void ABrandNewPlayerState::OnPlayerPawnPossessed(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	if (!IsValid(NewPawn) || !IsValid(Player) || GetWorld()->bIsTearingDown) return;
	
	bIsPawnSet = true;
	OnPlayerSetDelegate.ExecuteIfBound();
	
	
}

void ABrandNewPlayerState::Server_SetPlayerName_Implementation(const FString& NewName)
{
	Super::SetPlayerName(NewName);
	
}
