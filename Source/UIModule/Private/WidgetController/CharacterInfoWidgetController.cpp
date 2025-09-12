// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/CharacterInfoWidgetController.h"

#include "GameplayTagContainer.h"
#include "Interfaces/BrandNewPlayerInterface.h"

float UCharacterInfoWidgetController::GetAttributeByTag(const FGameplayTag& AttributeTag) const
{
	if (!IsValid(ControlledPawn) || !AttributeTag.IsValid()) return 0.f;

	const IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return 0.f;

	return PlayerInterface->GetAttributeValueByTag(AttributeTag);
}

void UCharacterInfoWidgetController::UpgradeAttribute(const TMap<FGameplayTag, float>& AttributeUpgradeMap)
{
	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return;

	TArray<FAttributeUpgradePrams> UpgradePrams;
	for (const TPair<FGameplayTag, float>& UpgradeData : AttributeUpgradeMap)
	{
		if (!UpgradeData.Key.IsValid()) continue;
		FAttributeUpgradePrams UpgradePram(UpgradeData.Key, UpgradeData.Value);

		UpgradePrams.Add(UpgradePram);
		
	}

	PlayerInterface->UpgradeAttribute(UpgradePrams);
	
}
