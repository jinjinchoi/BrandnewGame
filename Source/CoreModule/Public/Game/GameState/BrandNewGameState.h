﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BrandNewGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoin, const APlayerState*, NewPlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExit, const int32, ExitedPlayerId);

/**
 * 
 */
UCLASS()
class COREMODULE_API ABrandNewGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

	/* 플레이어가 참여할때 호출하는 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnPlayerJoin PlayerJoinDelegate;

	/* 플레이어가 세션에서 나갈때 호출하는 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnPlayerExit PlayerExitDelegate;
	
};
