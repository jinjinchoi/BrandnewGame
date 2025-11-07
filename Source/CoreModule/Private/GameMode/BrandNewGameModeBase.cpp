// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BrandNewGameModeBase.h"

#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "Manager/Pooling/BrandNewObjectPoolManager.h"

void ABrandNewGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	check(ObjectPoolManagerClass);

	if (!ObjectPoolManager)
	{
		ObjectPoolManager = NewObject<UBrandNewObjectPoolManager>(this, ObjectPoolManagerClass);
	}

	ObjectPoolManager->InitPoolManager();
	
}

void ABrandNewGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (bIsWaitingForTravel) return;

	const IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(Exiting->PlayerState);
	if (PlayerStateInterface) return;
	
	UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;
	SaveSubsystem->RemoveLatestPlayerData(PlayerStateInterface->GetPlayerUniqueId());
	
}
