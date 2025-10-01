// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "BrandNewPlayerState.generated.h"

class UBrandNewInventory;
class IBrandNewInventoryInterface;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerState : public APlayerState, public IBnPlayerStateInterface
{
	GENERATED_BODY()

public:
	ABrandNewPlayerState();

	virtual FInventoryContents GetInventoryContents() const override;

	UFUNCTION(BlueprintPure, Category = "Brandnew|Inventory")
	UBrandNewInventory* GetInventory() const; 

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBrandNewInventory> Inventory;
	
};
