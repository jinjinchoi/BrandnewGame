// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFunctionLibrary.h"

#include "Game/GameState/BrandNewGameState.h"
#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "HUD/BrandNewHUD.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

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

UInventoryWidgetController* UUIFunctionLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	if (const APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABrandNewHUD* HUD = Cast<ABrandNewHUD>(PlayerController->GetHUD()))
		{
			return HUD->GetInventoryWidgetController();
		}
	}

	return nullptr;
	
}

FSaveSlotViewInfoParams UUIFunctionLibrary::GetSaveSlotInfo(const UObject* WorldContextObject, const FString& SlotName, const int32 SlotIndex)
{
	if (!WorldContextObject) return FSaveSlotViewInfoParams();
	
	UBrandNewSaveSubsystem* SaveSubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return FSaveSlotViewInfoParams();
	
	const FSaveSlotPrams SaveSlotPrams = SaveSubsystem->GetSaveDataInSlot(SlotName, SlotIndex);
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
	if (!PlayerCharacter || !PlayerCharacter->HasAuthority() || !PlayerCharacter->GetWorld()) return;

	if (PlayerCharacter->IsNetMode(NM_Standalone))
	{
		if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(PlayerCharacter))
		{
			PlayerInterface->RequestSave(SlotName, SlotIndex);
		}
		return;
	}
	
	if (AGameStateBase* GameState = PlayerCharacter->GetWorld()->GetGameState())
	{
		for (const TObjectPtr<APlayerState>& PlayerState : GameState->PlayerArray)
		{
			if (!PlayerState) continue;
			
			APawn* Pawn = PlayerState->GetPawn();
			if (!Pawn)
			{
				UE_LOG(LogTemp, Warning, TEXT("RequestSave skipped: PlayerState %s has no Pawn."), *PlayerState->GetPlayerName());
				continue;
			}

			if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(Pawn))
			{
				PlayerInterface->RequestSave(SlotName, SlotIndex);
			}
		}
	}
}

FText UUIFunctionLibrary::GetFormattedItemDescription(const FText& OriginalDescription, const FItemDataRow& ItemData)
{
	const FFormatNamedArguments Args = MakeFormatArgsFromDataTableRow(ItemData);
	
	return FText::Format(OriginalDescription, Args);
}