// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/BrandNewFunctionLibrary.h"

#include "GameMode/BrandNewGameModeBase.h"

UBrandNewObjectPoolManager* UBrandNewFunctionLibrary::GetObjectPoolManager(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr) return nullptr;

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (World == nullptr) return nullptr;

	const ABrandNewGameModeBase* GameModeBase = Cast<ABrandNewGameModeBase>(World->GetAuthGameMode());
	if (GameModeBase == nullptr) return nullptr;

	return GameModeBase->GetObjectPoolManager();
	
}
