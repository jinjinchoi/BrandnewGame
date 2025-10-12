// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/GameOverWidgetController.h"

#include "Interfaces/Character/BrandNewPlayerInterface.h"

void UGameOverWidgetController::ReviveCharacter() const
{
	if (!ControlledPawn) return;

	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return;

	PlayerInterface->RevivePlayerCharacter();
	
}
