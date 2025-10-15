// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "DataAssets/DataAsset_AbilityInfo.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponChanged, const ECombatWeaponType, ChangedWeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOverlappedItemChanged, const bool, bIsOverlapped, const FPickupsUiInfo&, PickupsUiInfo);

class UBrandNewAttributeSet;
class UBrandNewAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class UIMODULE_API UOverlayWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() const override;
	virtual void BroadCastInitialValue() override;

	UFUNCTION(BlueprintPure, Category = "Brandnew|UtilityFunction")
	FAbilityInfoParams FindAbilityInfoByTag(const FGameplayTag& AbilityTagToFind) const;
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FWeaponChanged OnWeaponChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnOverlappedItemChanged OnOverlappedItemChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|DataAssets")
	TObjectPtr<UDataAsset_AbilityInfo> AbilityInfoDataAsset;

	UPROPERTY(BlueprintReadOnly, Category = "Brandnew|GameState")
	AGameStateBase* GameState;

public:
	FORCEINLINE void SetGameState(AGameStateBase* GameStateToSet) { GameState = GameStateToSet; }

	
};
