// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BrandNewGameState.generated.h"

/**
 * 
 */
UCLASS()
class COREMODULE_API ABrandNewGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	TArray<APawn*> GetTeammates();
	
};
