// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BrandNewHUD.h"

#include "Widget/BrandNewWidget.h"
#include "WidgetController/OverlayWidgetController.h"

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
		OverlayWidgetController->BindCallbacksToDependencies(GetOwningPlayerController()->GetPawn());
		OverlayWidgetController->SetControlledPawn(GetOwningPlayerController()->GetPawn());
	}

	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidget->AddToViewport();
	
}

void ABrandNewHUD::RequestInitHUD()
{
	InitHUD();
}
