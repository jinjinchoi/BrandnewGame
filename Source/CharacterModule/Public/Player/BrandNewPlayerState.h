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
	virtual void OnRep_PlayerId() override;
	
	/* begin IBnPlayerStateInterface */
	virtual FInventoryContents GetInventoryContents() const override;
	virtual bool IsPlayerReplicated() const override;
	virtual FOnPlayerSetDelegate& GetPlayerSetDelegate() override;
	virtual void SetPlayerNameToPlayerState(const FString& NewName) override;
	/* end IBnPlayerStateInterface */

	UFUNCTION(BlueprintPure, Category = "Brandnew|Inventory")
	UBrandNewInventory* GetInventory() const;
	
	bool bIsPawnSet = false;
	bool bIsPlayerNameSet = false;

	FOnPlayerSetDelegate OnPlayerSetDelegate;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBrandNewInventory> Inventory;

	/* Pawn하고 PlayerName 모두 설정 되면 Game State에 델리게이트 알림.  */
	void NotifyWhenPawnReady();

	UFUNCTION()
	void OnPlayerPawnPossessed(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);
	
};
