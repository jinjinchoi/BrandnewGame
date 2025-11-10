// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "BrandNewPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerIdSet, const FString, InPlayerId);

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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/* begin IBnPlayerStateInterface */
	virtual FInventoryContents GetInventoryContents() const override;
	virtual FString GetPlayerUniqueId() const override;
	/* end IBnPlayerStateInterface */

	UFUNCTION(BlueprintPure, Category = "Brandnew|Inventory")
	UBrandNewInventory* GetInventory() const;

	UPROPERTY(ReplicatedUsing = "OnRep_PlayerUniqueId", BlueprintReadOnly)
	FString PlayerUniqueId;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FOnPlayerIdSet OnPlayerIdSetDelegate;

private:
	UFUNCTION()
	void OnRep_PlayerUniqueId();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBrandNewInventory> Inventory;
	
};
