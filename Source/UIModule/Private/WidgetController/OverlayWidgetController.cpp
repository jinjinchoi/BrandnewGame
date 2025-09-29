// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/OverlayWidgetController.h"

#include "Interfaces/Character/BrandNewPlayerInterface.h"


void UOverlayWidgetController::BindCallbacksToDependencies(APawn* InControlledPawn) const
{
	if (!IsValid(InControlledPawn)) return;

	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(InControlledPawn);
	if (!PlayerInterface) return;

	PlayerInterface->GetHealthChangedDelegate().BindLambda([this](const float NewValue)
	{
		OnHealthChangedDelegate.Broadcast(NewValue);
	});
	
	PlayerInterface->GetMaxHealthChangedDelegate().BindLambda([this](const float NewValue)
	{
		OnMaxHealthChangedDelegate.Broadcast(NewValue);
	});

	PlayerInterface->GetManaChangedDelegate().BindLambda([this](const float NewValue)
	{
		OnManaChangedDelegate.Broadcast(NewValue);	
	});

	PlayerInterface->GetMaxManaChangedDelegate().BindLambda([this](const float NewValue)
	{
		OnMaxManaChangedDelegate.Broadcast(NewValue);
	});

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

	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn);
	if (!PlayerInterface) return FoundAbilityInfoParams;

	FoundAbilityInfoParams.RequiredMana = PlayerInterface->GetRequiredAbilityMana(AbilityTagToFind);
	return FoundAbilityInfoParams;
	
	
}
