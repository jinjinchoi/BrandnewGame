// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BnMainMenuHUD.h"

#include "Blueprint/UserWidget.h"

void ABnMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	check(MainMenuWidgetClass)

	if (!MainMenuWidget)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
	}
	
}
