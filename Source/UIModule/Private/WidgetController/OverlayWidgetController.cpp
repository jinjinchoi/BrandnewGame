// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/OverlayWidgetController.h"

#include "Interfaces/Character/BrandNewPlayerInterface.h"


void UOverlayWidgetController::BindCallbacksToDependencies(APawn* InControlledPawn) const
{
	if (!IsValid(InControlledPawn)) return;

	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(InControlledPawn);
	if (!PlayerInterface) return;

	PlayerInterface->GetWeaponChangedDelegate().BindLambda([this](const ECombatWeaponType ChangedWeaponType)
	{
		OnWeaponChangedDelegate.Broadcast(ChangedWeaponType);
	});
	PlayerInterface->GetOnOverlapChangedDelegate().BindLambda([this]( const bool bIsBeginOverlap, const FPickupsUiInfo& PickupsUiInfo)
	{
		OnOverlappedItemChangedDelegate.Broadcast(bIsBeginOverlap, PickupsUiInfo);
	});
	
}

void UOverlayWidgetController::BroadCastInitialValue()
{
	if (!IsValid(ControlledPawn)) return;

	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return;

	PlayerInterface->RequestBroadCastAttributeValue();
	
	
}

FAbilityInfoParams UOverlayWidgetController::FindAbilityInfoByTag(const FGameplayTag& AbilityTagToFind) const
{
	check(AbilityInfoDataAsset)
	
	FAbilityInfoParams FoundAbilityInfoParams = AbilityInfoDataAsset->FindAbilityInfoByAbilityTag(AbilityTagToFind);

	const IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return FoundAbilityInfoParams;

	FoundAbilityInfoParams.RequiredMana = PlayerInterface->GetRequiredAbilityMana(AbilityTagToFind);
	return FoundAbilityInfoParams;
	
}