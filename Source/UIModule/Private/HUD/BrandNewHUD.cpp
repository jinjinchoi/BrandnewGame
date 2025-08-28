// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BrandNewHUD.h"

#include "Widget/BrandNewWidget.h"

void ABrandNewHUD::InitHUD()
{
	check(OverlayWidgetClass)
	
	if (!OverlayWidget)
	{
		OverlayWidget = CreateWidget<UBrandNewWidget>(GetWorld(), OverlayWidgetClass);
	}

	OverlayWidget->AddToViewport();
	
}
