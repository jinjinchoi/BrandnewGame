// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interfaces/BrandNewHUDInterface.h"
#include "BrandNewHUD.generated.h"

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
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget")
	TSubclassOf<UBrandNewWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UBrandNewWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Widget Controller")
	TSubclassOf<UCharacterInfoWidgetController> CharacterInfoWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UCharacterInfoWidgetController> CharacterInfoWidgetController;
	
};
