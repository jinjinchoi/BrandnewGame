// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/UI/BrandNewHUDInterface.h"
#include "BrandNewHUD.generated.h"

class UQuestWidgetController;
class UDialogueWidgetController;
class UGameOverWidgetController;
class UInventoryWidgetController;
class UCharacterInfoWidgetController;
class UOverlayWidgetController;
class UBrandNewWidget;
/**
 * 실제 In game에서 사용하는 HUD
 */
UCLASS()
class UIMODULE_API ABrandNewHUD : public AHUD, public IBrandNewHUDInterface
{
	GENERATED_BODY()

public:
	void InitHUD();

	/* begin IBrandNewHUDInterface */
	virtual void RequestInitHUD() override;
	virtual void CreateDialogueWidget(const FName& FirstDialogueId) override;
	virtual void HideMainOverlay() override;
	virtual void CreateSequenceOverlayWidget() override;
	virtual void RemoveSequenceOverlayWidget() override;
	virtual void ShowMainOverlay_Implementation() override;
	/* end IBrandNewHUDInterface */

	UCharacterInfoWidgetController* GetCharacterInfoWidgetController();
	UInventoryWidgetController* GetInventoryWidgetController();
	UGameOverWidgetController* GetGameOverWidgetController();
	UQuestWidgetController* GetQuestWidgetController();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget")
	TSubclassOf<UBrandNewWidget> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget")
	TSubclassOf<UBrandNewWidget> DialogueWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Widget")
	TSubclassOf<UUserWidget> SequenceOverlayWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget Controller")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget Controller")
	TSubclassOf<UCharacterInfoWidgetController> CharacterInfoWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget Controller")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget Controller")
	TSubclassOf<UGameOverWidgetController> GameOverWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget Controller")
	TSubclassOf<UDialogueWidgetController> DialogueWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Widget Controller")
	TSubclassOf<UQuestWidgetController> QuestWidgetControllerClass;

private:
	UPROPERTY()
	TObjectPtr<UBrandNewWidget> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UBrandNewWidget> DialogueWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> SequenceOverlayWidget;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY()
	TObjectPtr<UCharacterInfoWidgetController> CharacterInfoWidgetController;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY()
	TObjectPtr<UGameOverWidgetController> GameOverWidgetController;

	UPROPERTY()
	TObjectPtr<UDialogueWidgetController> DialogueWidgetController;
	
	UPROPERTY()
	TObjectPtr<UQuestWidgetController> QuestWidgetController;
	
};
