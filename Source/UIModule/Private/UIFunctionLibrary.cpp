// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFunctionLibrary.h"

#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "HUD/BrandNewHUD.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "GameFramework/Character.h"

UCharacterInfoWidgetController* UUIFunctionLibrary::GetCharacterInfoWidgetController(const UObject* WorldContextObject)
{
	if (const APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABrandNewHUD* HUD = Cast<ABrandNewHUD>(PlayerController->GetHUD()))
		{
			return HUD->GetCharacterInfoWidgetController();
		}
	}

	return nullptr;
}

FSaveSlotViewInfoParams UUIFunctionLibrary::GetSaveSlotInfo(const FString& SlotName, const int32 SlotIndex)
{
	const FSaveSlotPrams SaveSlotPrams = UBrandNewSaveSubsystem::GetSaveDataInSlot(SlotName, SlotIndex);
	if (SaveSlotPrams.bIsValid)
	{
		FSaveSlotViewInfoParams SlotViewParams;
		SlotViewParams.Level = SaveSlotPrams.AttributePrams.Level;
		SlotViewParams.Date = SaveSlotPrams.SavedTime;
		SlotViewParams.LocationName = SaveSlotPrams.MapName;
		SlotViewParams.TitleText = SaveSlotPrams.TitleText;
		SlotViewParams.bIsValid = true;

		return SlotViewParams;
	}

	return FSaveSlotViewInfoParams();
}

void UUIFunctionLibrary::RequestSave(ACharacter* PlayerCharacter, const FString& SlotName, const int32 SlotIndex)
{
	check(PlayerCharacter)

	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(PlayerCharacter))
	{
		PlayerInterface->RequestSave(SlotName, SlotIndex);
	}
}
