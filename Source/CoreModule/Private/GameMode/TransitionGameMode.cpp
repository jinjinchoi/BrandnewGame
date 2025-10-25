// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TransitionGameMode.h"

#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"
#include "Interfaces/Player/UIPlayerControllerInterface.h"

void ATransitionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	const FString TargetLevelPath = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>()->GetTraveledLevelPath().ToString();

	if (IUIPlayerControllerInterface* UIPlayerController = Cast<IUIPlayerControllerInterface>(NewPlayer))
	{
		UIPlayerController->SetTraveledMapPathToClient(TargetLevelPath);
	}
	
}
