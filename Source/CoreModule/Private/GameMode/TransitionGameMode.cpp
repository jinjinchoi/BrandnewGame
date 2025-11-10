// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TransitionGameMode.h"

#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"
#include "Interfaces/Player/UIPlayerControllerInterface.h"


void ATransitionGameMode::Logout(AController* Exiting)
{

	if (const APlayerController* PC = Cast<APlayerController>(Exiting))
	{
		GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>()->UnregisterPlayerLoaded(PC);
	}
	
	Super::Logout(Exiting);
}
