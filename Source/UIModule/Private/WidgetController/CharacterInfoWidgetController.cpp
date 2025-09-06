// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/CharacterInfoWidgetController.h"

#include "Interfaces/BrandNewPlayerInterface.h"

float UCharacterInfoWidgetController::GetAttributeByTag(const FGameplayTag& AttributeTag) const
{
	if (IsValid(ControlledPawn)) return 0.f;

	const IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return 0.f;

	return PlayerInterface->GetAttributeByTag(AttributeTag);
}
