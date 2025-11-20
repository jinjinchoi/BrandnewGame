// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/BrandNewGameState.h"

void ABrandNewGameState::RegisterPlayerState(APlayerState* NewPlayerState)
{
	PlayerStateArray.AddUnique(NewPlayerState);
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