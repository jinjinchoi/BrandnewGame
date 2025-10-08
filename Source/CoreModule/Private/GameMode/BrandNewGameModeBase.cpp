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

void ABrandNewGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;

	APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();
	if (!PS) return;
	IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(PS);
	if (!PlayerStateInterface) return;
	PlayerStateInterface->SetPlayerNameToPlayerState(SaveSubsystem->GetUniqueIdentifier());
	
	
	
}
