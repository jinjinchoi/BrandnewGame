// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/UI/BrandNewHUDInterface.h"
#include "BrandNewHUD.generated.h"

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
	virtual void ShowMainOverlay_Implementation() override;
	/* end IBrandNewHUDInterface */

	UCharacterInfoWidgetController* GetCharacterInfoWidgetController();
	UInventoryWidgetController* GetInventoryWidgetController();
	UGameOverWidgetController* GetGameOverWidgetController();
	
protected:
	virtual void BeginPlay() override;
	void OnCinematicPlaying(const bool bIsPlaying);
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget")
	TSubclassOf<UBrandNewWidget> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget")
	TSubclassOf<UBrandNewWidget> DialogueWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UCharacterInfoWidgetController> CharacterInfoWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UGameOverWidgetController> GameOverWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UDialogueWidgetController> DialogueWidgetControllerClass;

private:
	UPROPERTY()
	TObjectPtr<UBrandNewWidget> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UBrandNewWidget> DialogueWidget;

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
	
};
