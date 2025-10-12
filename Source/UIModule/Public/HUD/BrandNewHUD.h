// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/UI/BrandNewHUDInterface.h"
#include "BrandNewHUD.generated.h"

class UGameOverWidgetController;
class UInventoryWidgetController;
class UCharacterInfoWidgetController;
class UOverlayWidgetController;
class UBrandNewWidget;
/**
 * 
 */
UCLASS()
class UIMODULE_API ABrandNewHUD : public AHUD, public IBrandNewHUDInterface
{
	GENERATED_BODY()

public:
	void InitHUD();

	/* begin IBrandNewHUDInterface */
	virtual void RequestInitHUD() override;
	/* end IBrandNewHUDInterface */

	UCharacterInfoWidgetController* GetCharacterInfoWidgetController();
	UInventoryWidgetController* GetInventoryWidgetController();
	UGameOverWidgetController* GetGameOverWidgetController();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget")
	TSubclassOf<UBrandNewWidget> OverlayWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UCharacterInfoWidgetController> CharacterInfoWidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UGameOverWidgetController> GameOverWidgetControllerClass;

private:
	UPROPERTY()
	TObjectPtr<UBrandNewWidget> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY()
	TObjectPtr<UCharacterInfoWidgetController> CharacterInfoWidgetController;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY()
	TObjectPtr<UGameOverWidgetController> GameOverWidgetController;
	
};
