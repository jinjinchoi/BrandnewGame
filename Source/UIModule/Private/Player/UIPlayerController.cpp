// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UIPlayerController.h"

#include "Game/GameState/BrandNewGameState.h"
#include "GameFramework/PlayerState.h"

void AUIPlayerController::K2_NotifyMapLoaded()
{
	if (HasAuthority())
	{
		NotifyMapLoaded();
	}
	else
	{
		Server_NotifyMapLoaded();
	}	
}

void AUIPlayerController::Server_NotifyMapLoaded_Implementation()
{
	NotifyMapLoaded();
	
}

void AUIPlayerController::NotifyMapLoaded() const
{
	if (ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState()))
	{
		const int32 PlayerId = GetPlayerState<APlayerState>()->GetPlayerId();
		BrandNewGameState->RegisterPlayerLoaded(PlayerId);
	}
}
