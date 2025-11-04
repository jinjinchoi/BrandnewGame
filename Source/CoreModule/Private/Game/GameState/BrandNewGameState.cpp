// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/BrandNewGameState.h"
#include "Net/UnrealNetwork.h"

void ABrandNewGameState::RegisterPlayerState(APlayerState* NewPlayerState)
{
	if (HasAuthority())
	{
		PlayerStateArray.AddUnique(NewPlayerState);
		OnSetPlayerCharacterDelegate.Broadcast();
	}
}
void ABrandNewGameState::OnRep_PlayerStateArray()
{
	OnSetPlayerCharacterDelegate.Broadcast();
}

void ABrandNewGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	
	PlayerJoinDelegate.Broadcast(PlayerState);
	
}

void ABrandNewGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	
	PlayerExitDelegate.Broadcast(PlayerState);
	
}

void ABrandNewGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerStateArray);
	
}
