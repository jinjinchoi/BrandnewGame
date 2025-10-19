// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/UI/EntryStatusWidgetInterface.h"
#include "RegionEntryStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIMODULE_API URegionEntryStatusWidget : public UUserWidget, public IEntryStatusWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void SendCurrentOverlappedPlayersNum_Implementation(const int32 NumOfOverlappedPlayer) override;
	virtual void SendMaxPlayersNum_Implementation(const int32 NumOfMaxPlayers) override;
	
};
