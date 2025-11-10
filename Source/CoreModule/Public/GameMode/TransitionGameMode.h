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
	/* 플레이어가 로딩 중 나갈 경우 로드 완료된 클라이언트 목록에서 제외 */
	virtual void Logout(AController* Exiting) override;
};
