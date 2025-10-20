// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BrandNewHUD.h"

#include "Game/GameInstance/BrandNewGameInstance.h"
#include "Manager/Sequnce/SequenceManager.h"
#include "Widget/BrandNewWidget.h"
#include "WidgetController/CharacterInfoWidgetController.h"
#include "WidgetController/DialogueWidgetController.h"
#include "WidgetController/InventoryWidgetController.h"
#include "WidgetController/OverlayWidgetController.h"
#include "WidgetController/GameOverWidgetController.h"



void ABrandNewHUD::BeginPlay()
{
	Super::BeginPlay();

	if (USequenceManager* SequenceManager = GetGameInstance<UBrandNewGameInstance>()->GetSequenceManager())
	{
		SequenceManager->OnSequencePlayStateChangedDelegate.AddWeakLambda(this, [this](const bool bIsPlaying)
		{
			if (this->OverlayWidget)
			{
				this->OverlayWidget->SetVisibility(bIsPlaying ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
			}
		});
	}
}

void ABrandNewHUD::RequestInitHUD()
{
	InitHUD();
}

void ABrandNewHUD::InitHUD()
{
	check(OverlayWidgetClass)
	
	if (!OverlayWidget)
	{
		OverlayWidget = CreateWidget<UBrandNewWidget>(GetWorld(), OverlayWidgetClass);
	}
	
	if (!OverlayWidgetController && GetOwningPlayerController() && GetOwningPlayerController()->GetPawn())
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetControlledPawn(GetOwningPlayerController()->GetPawn());
		OverlayWidgetController->SetGameState(GetOwningPlayerController()->GetWorld()->GetGameState());
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidget->AddToViewport();
	
}

void ABrandNewHUD::HideMainOverlay()
{
	if (!OverlayWidget) return;

	OverlayWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ABrandNewHUD::ShowMainOverlay_Implementation()
{
	if (!OverlayWidget) return;

	OverlayWidget->SetVisibility(ESlateVisibility::Visible);
	
}

void ABrandNewHUD::CreateDialogueWidget(const FName& FirstDialogueId)
{
	check(DialogueWidgetControllerClass && DialogueWidgetClass);
	if (!DialogueWidgetControllerClass || !DialogueWidgetClass) return;
	
	if (!DialogueWidgetController)
	{
		DialogueWidgetController = NewObject<UDialogueWidgetController>(this, DialogueWidgetControllerClass);
		DialogueWidgetController->SetControlledPawn(GetOwningPlayerController()->GetPawn());
		DialogueWidgetController->BindCallbacksToDependencies();
	}

	if (!DialogueWidget)
	{
		DialogueWidget = CreateWidget<UBrandNewWidget>(GetWorld(), DialogueWidgetClass);
	}
	
	DialogueWidgetController->DialogueId = FirstDialogueId;
	DialogueWidget->SetWidgetController(DialogueWidgetController);
	DialogueWidget->AddToViewport();
	
}

UCharacterInfoWidgetController* ABrandNewHUD::GetCharacterInfoWidgetController()
{
	check(CharacterInfoWidgetControllerClass)
	
	if (!CharacterInfoWidgetController)
	{
		CharacterInfoWidgetController = NewObject<UCharacterInfoWidgetController>(this, CharacterInfoWidgetControllerClass);
		CharacterInfoWidgetController->SetControlledPawn(GetOwningPlayerController()->GetPawn());
	}

	return CharacterInfoWidgetController;
	
}

UInventoryWidgetController* ABrandNewHUD::GetInventoryWidgetController()
{
	check(InventoryWidgetControllerClass)

	if (!InventoryWidgetController)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetControlledPawn(GetOwningPlayerController()->GetPawn());
		InventoryWidgetController->SetPlayerState(GetOwningPlayerController()->PlayerState);
		
	}

	return InventoryWidgetController;
	
}

UGameOverWidgetController* ABrandNewHUD::GetGameOverWidgetController()
{
	check(GameOverWidgetControllerClass)

	if (!GameOverWidgetController)
	{
		GameOverWidgetController = NewObject<UGameOverWidgetController>(this, GameOverWidgetControllerClass);
		GameOverWidgetController->SetControlledPawn(GetOwningPlayerController()->GetPawn());
	}

	return GameOverWidgetController;
	
}

