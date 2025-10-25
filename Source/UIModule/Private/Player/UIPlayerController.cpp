// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UIPlayerController.h"

#include "Game/GameState/BrandNewGameState.h"
#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"
#include "GameFramework/PlayerState.h"

void AUIPlayerController::SetTraveledMapPathToClient(const FString& MapPath)
{
	if (!HasAuthority()) return;
	
	Client_SetTraveledMapPath(MapPath);
	
}

void AUIPlayerController::Client_SetTraveledMapPath_Implementation(const FString& MapPath)
{
	if (UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>())
	{
		LevelManagerSubsystem->SetMapNameToTravelByString(MapPath);
		LevelManagerSubsystem->StartAsyncLoading();
	}
	
}


void AUIPlayerController::BP_NotifyMapLoaded()
{
	const int32 PlayerId = GetPlayerState<APlayerState>()->GetPlayerId();
	
	Server_NotifyMapLoaded(PlayerId);

}

void AUIPlayerController::Server_NotifyMapLoaded_Implementation(const int32 PlayerId)
{
	if (ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState()))
	{
		BrandNewGameState->RegisterPlayerLoaded(PlayerId);
	}
	
}
