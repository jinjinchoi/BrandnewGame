// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BrandNewPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABrandNewPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|SubClass", meta = (MustImplement = "BrandNewInventoryInterface"))
	TSubclassOf<UObject> InventoryClass;

private:
	void SetupInventory();
	
	UPROPERTY(Replicated)
	TObjectPtr<UObject> Inventory;
	
};
