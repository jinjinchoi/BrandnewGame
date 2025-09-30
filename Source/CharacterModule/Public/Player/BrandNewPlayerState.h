// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BrandNewPlayerState.generated.h"

class UBrandNewInventory;
class IBrandNewInventoryInterface;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABrandNewPlayerState();
	UBrandNewInventory* GetInventoryInterfaceClass() const; 

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBrandNewInventory> Inventory;
	
};
