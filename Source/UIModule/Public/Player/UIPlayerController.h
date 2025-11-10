// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Player/UIPlayerControllerInterface.h"
#include "UIPlayerController.generated.h"

/**
 * 캐릭터가 없는 단순 UI 화면 일때만 사용하는 플레이어 컨트롤러.
 * 대표적으로 트랜지션 맵이나 메인 화면.
 */
UCLASS()
class UIMODULE_API AUIPlayerController : public APlayerController, public IUIPlayerControllerInterface
{
	GENERATED_BODY()

public:
	/* 맵 로드가 완료될때 레벨 매니저 서브시스템에 알리는 서버함수 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Brandnew|Map")
	void Server_NotifyMapLoaded();

	/* 접속한 플레이어들이 각각 비동기적으로 에셋 로드하도록 요청하는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Loading")
	void StartAsyncLoading();
	
	
	
};
