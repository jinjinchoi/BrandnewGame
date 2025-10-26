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
	/**
	 * 트랜지션 맵으로 이동할때 클라이언트는 맵 에셋 네임을 모르고 그렇기 때문에
	 * 서버에서 클라이언트에 RPC를 통해 알려줘 클라이언트도 로드를 진행할 수 있게 해야함
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/* 플레이어가 로딩 중 나갈 경우 로드 완료된 클라이언트 목록에서 제외 */
	virtual void Logout(AController* Exiting) override;
};
