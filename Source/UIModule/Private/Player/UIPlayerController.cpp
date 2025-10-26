// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UIPlayerController.h"
#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"


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


void AUIPlayerController::Server_NotifyMapLoaded_Implementation()
{
	if (UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>())
	{
		LevelManagerSubsystem->RegisterPlayerLoaded(this);
	}
}
