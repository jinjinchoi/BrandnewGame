// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BrandNewWidget.h"

void UBrandNewWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
	
}

void UBrandNewWidget::SetUIWidgetController(UObject* ControllerToSet)
{
	SetWidgetController(ControllerToSet);
}

