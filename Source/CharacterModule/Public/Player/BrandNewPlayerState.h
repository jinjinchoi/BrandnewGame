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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	/* begin IBnPlayerStateInterface */
	virtual FInventoryContents GetInventoryContents() const override;
	virtual FString GetPlayerUniqueId() const override;
	/* end IBnPlayerStateInterface */

	UFUNCTION(BlueprintPure, Category = "Brandnew|Inventory")
	UBrandNewInventory* GetInventory() const;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FString PlayerUniqueId;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBrandNewInventory> Inventory;

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerUniqueId(const FString& NewPlayerUniqueId);
	
};
