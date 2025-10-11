// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandNewSaveSubsystem.h"

#include "Game/Save/BrandNewSlotSaveGame.h"
#include "Kismet/GameplayStatics.h"


void UBrandNewSaveSubsystem::SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams) const
{
	SaveGameToSlotWithId(SlotName, SlotIndex, SaveSlotPrams, UniqueIdentifier);
}

void UBrandNewSaveSubsystem::SaveGameToSlotWithId(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams, const FString& UniqueId)
{
	const FString UniqueSlotName = SlotName + UniqueId;
	
	UBrandNewSlotSaveGame* SlotSaveGame = Cast<UBrandNewSlotSaveGame>(UGameplayStatics::CreateSaveGameObject(UBrandNewSlotSaveGame::StaticClass()));
	SlotSaveGame->AttributePrams = SaveSlotPrams.AttributePrams;
	SlotSaveGame->AbilityDataMap = SaveSlotPrams.AbilityMap;
	SlotSaveGame->CharacterLocation = SaveSlotPrams.CharacterLocation;
	SlotSaveGame->CurrentMapName = SaveSlotPrams.MapName;
	SlotSaveGame->SavedTime = SaveSlotPrams.SavedTime;
	SlotSaveGame->TitleText = SaveSlotPrams.TitleText;
	SlotSaveGame->MapPackageName = SaveSlotPrams.MapPackageName;
	SlotSaveGame->Inventory = SaveSlotPrams.InventoryContents;

	UGameplayStatics::SaveGameToSlot(SlotSaveGame, UniqueSlotName, SlotIndex);
}

FSaveSlotPrams UBrandNewSaveSubsystem::GetSaveDataById(const FString& SlotName, const int32 SlotIndex, const FString& UniqueId) const
{
	const FString UniqueSlotName = SlotName + UniqueId;
	
	if (UGameplayStatics::DoesSaveGameExist(UniqueSlotName, SlotIndex))
	{
		const UBrandNewSlotSaveGame* SlotSaveGame = Cast<UBrandNewSlotSaveGame>(UGameplayStatics::LoadGameFromSlot(UniqueSlotName, SlotIndex));
		if (!SlotSaveGame) return FSaveSlotPrams();

		FSaveSlotPrams SaveSlotPrams;
		SaveSlotPrams.AbilityMap = SlotSaveGame->AbilityDataMap;
		SaveSlotPrams.AttributePrams = SlotSaveGame->AttributePrams;
		SaveSlotPrams.CharacterLocation = SlotSaveGame->CharacterLocation;
		SaveSlotPrams.MapName = SlotSaveGame->CurrentMapName;
		SaveSlotPrams.SavedTime = SlotSaveGame->SavedTime;
		SaveSlotPrams.TitleText = SlotSaveGame->TitleText;
		SaveSlotPrams.MapPackageName = SlotSaveGame->MapPackageName;
		SaveSlotPrams.InventoryContents = SlotSaveGame->Inventory;
		SaveSlotPrams.bIsValid = true;
		
		return SaveSlotPrams;
	}

	return FSaveSlotPrams();
}

FSaveSlotPrams UBrandNewSaveSubsystem::GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex) const
{
	return GetSaveDataById(SlotName, SlotIndex, UniqueIdentifier);
}

FSaveSlotPrams UBrandNewSaveSubsystem::GetSaveDataInCurrentSlot() const
{
	return GetSaveDataInSlot(CurrentSlotName, CurrentSlotIndex);
}

FSaveSlotPrams UBrandNewSaveSubsystem::GetCurrentSlotSaveDataById(const FString& UniqueId) const
{
	return GetSaveDataById(CurrentSlotName, CurrentSlotIndex, UniqueId);
}

void UBrandNewSaveSubsystem::SavePlayerData(const FSaveSlotPrams& SaveSlotPrams)
{
	LastestPlayerData = SaveSlotPrams;
}

FSaveSlotPrams UBrandNewSaveSubsystem::GetLastestPlayerData()
{
	return LastestPlayerData;
}

void UBrandNewSaveSubsystem::ResetPlayerData()
{
	LastestPlayerData = FSaveSlotPrams();
	UniqueIdentifier.Empty();
}

FString UBrandNewSaveSubsystem::TryGetMapAssetNameAndSaveSlotInfo(const FString& SlotName, const int32 SlotIndex)
{
	const FSaveSlotPrams SaveSlotPrams = GetSaveDataInSlot(SlotName, SlotIndex);
	bIsLoadedWorld = SaveSlotPrams.bIsValid;
	
	if (SaveSlotPrams.bIsValid)
	{
		SetCurrentSlotNameAndIndex(SlotName, SlotIndex);
		return SaveSlotPrams.MapPackageName;
	}

	return FString();
	
}

void UBrandNewSaveSubsystem::SetCurrentSlotNameAndIndex(const FString& SlotName, const int32 SlotIndex)
{
	CurrentSlotName = SlotName;
	CurrentSlotIndex = SlotIndex;
}

void UBrandNewSaveSubsystem::Login(const FString& Id)
{
	UniqueIdentifier = Id;
}

FString UBrandNewSaveSubsystem::GetUniqueIdentifier() const
{
	return UniqueIdentifier;
}
