// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TransitionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COREMODULE_API ATransitionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/* 트랜지션 맵에 있을때 플레이어가 접속하면 이동할 Map Path 통보 */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
