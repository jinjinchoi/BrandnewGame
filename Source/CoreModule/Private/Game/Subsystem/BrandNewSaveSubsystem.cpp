// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandNewSaveSubsystem.h"

#include "Game/Save/BrandNewSlotSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UBrandNewSaveSubsystem::SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams)
{
	const FString UniqueSlotName = SlotName + UniqueIdentifier;
	
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

FSaveSlotPrams UBrandNewSaveSubsystem::GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex)
{
	const FString UniqueSlotName = SlotName + UniqueIdentifier;
	
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
}

FString UBrandNewSaveSubsystem::RequestCharacterDataLoad(const FString& SlotName, const int32 SlotIndex)
{
	const FSaveSlotPrams SaveSlotPrams = GetSaveDataInSlot(SlotName, SlotIndex);
	if (SaveSlotPrams.bIsValid)
	{
		SavePlayerData(SaveSlotPrams);
		return SaveSlotPrams.MapPackageName;
	}

	return FString();
	
}

void UBrandNewSaveSubsystem::Login(const FString& Id)
{
	UniqueIdentifier = Id;
}

FString UBrandNewSaveSubsystem::GetUniqueIdentifier() const
{
	return UniqueIdentifier;
}
