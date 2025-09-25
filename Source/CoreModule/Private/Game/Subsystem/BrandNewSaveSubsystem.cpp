// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandNewSaveSubsystem.h"

#include "Game/Save/BrandNewSlotSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UBrandNewSaveSubsystem::SaveGameToSlot(const FString& SlotName, const int32 SlotIndex, const FSaveSlotPrams& SaveSlotPrams)
{
	UBrandNewSlotSaveGame* SlotSaveGame = Cast<UBrandNewSlotSaveGame>(UGameplayStatics::CreateSaveGameObject(UBrandNewSlotSaveGame::StaticClass()));
	SlotSaveGame->AttributePrams = SaveSlotPrams.AttributePrams;
	SlotSaveGame->AbilityDataMap = SaveSlotPrams.AbilityMap;
	SlotSaveGame->CharacterLocation = SaveSlotPrams.CharacterLocation;
	SlotSaveGame->CurrentMapName = SaveSlotPrams.MapName;
	SlotSaveGame->SavedTime = SaveSlotPrams.SavedTime;
	SlotSaveGame->TitleText = SaveSlotPrams.TitleText;
	SlotSaveGame->MapPackageName = SaveSlotPrams.MapPackageName;

	UGameplayStatics::SaveGameToSlot(SlotSaveGame, SlotName, SlotIndex);
	
	
}

FSaveSlotPrams UBrandNewSaveSubsystem::GetSaveDataInSlot(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		const UBrandNewSlotSaveGame* SlotSaveGame = Cast<UBrandNewSlotSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
		if (!SlotSaveGame) return FSaveSlotPrams();

		FSaveSlotPrams SaveSlotPrams;
		SaveSlotPrams.AbilityMap = SlotSaveGame->AbilityDataMap;
		SaveSlotPrams.AttributePrams = SlotSaveGame->AttributePrams;
		SaveSlotPrams.CharacterLocation = SlotSaveGame->CharacterLocation;
		SaveSlotPrams.MapName = SlotSaveGame->CurrentMapName;
		SaveSlotPrams.SavedTime = SlotSaveGame->SavedTime;
		SaveSlotPrams.TitleText = SlotSaveGame->TitleText;
		SaveSlotPrams.MapPackageName = SlotSaveGame->MapPackageName;
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
