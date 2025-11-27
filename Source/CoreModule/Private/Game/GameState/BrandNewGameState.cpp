// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/BrandNewGameState.h"
#include "GameFramework/PlayerState.h"


void ABrandNewGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (PlayerState->GetPawn())
	{
		PlayerJoinDelegate.Broadcast(PlayerState);
	}
	else
	{
		PlayerState->OnPawnSet.AddDynamic(this, &ThisClass::OnPlayerPawnSet);
	}
	
	
}

void ABrandNewGameState::OnPlayerPawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	PlayerJoinDelegate.Broadcast(Player);
}

void ABrandNewGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	
	PlayerExitDelegate.Broadcast(PlayerState);
	
}

