// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UIPlayerController.h"
#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"


void AUIPlayerController::Server_NotifyMapLoaded_Implementation()
{
	if (UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>())
	{
		LevelManagerSubsystem->RegisterPlayerLoaded(this);
	}
}

void AUIPlayerController::StartAsyncLoading()
{
	if (!IsLocalController()) return;

	if (UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>())
	{
		LevelManagerSubsystem->StartAsyncLoading();
	}
}
