// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/BrandNewGameState.h"
#include "GameFramework/PlayerState.h"

TArray<APawn*> ABrandNewGameState::GetTeammates()
{
	TArray<APawn*> Result;
	
	for (const APlayerState* PS : PlayerArray)
	{
		if (PS && PS->GetPawn())
		{
			Result.Add(PS->GetPawn());
		}
	}

	return Result;
}
