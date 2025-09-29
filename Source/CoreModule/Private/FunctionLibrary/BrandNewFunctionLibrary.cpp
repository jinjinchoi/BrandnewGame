// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/BrandNewFunctionLibrary.h"

#include "Game/GameInstance/BrandNewGameInstance.h"
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

FItemDataRow UBrandNewFunctionLibrary::GetItemData(const UObject* WorldContextObject, const int32 ItemId)
{
	if (WorldContextObject == nullptr) return FItemDataRow();

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return FItemDataRow();

	if (UBrandNewGameInstance* BrandNewGameInstance =  Cast<UBrandNewGameInstance>(World->GetGameInstance()))
	{
		BrandNewGameInstance->GetItemData(ItemId);
	}

	return FItemDataRow();
}
