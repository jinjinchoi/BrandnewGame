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