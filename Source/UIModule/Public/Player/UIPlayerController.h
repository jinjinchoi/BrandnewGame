// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Player/UIPlayerControllerInterface.h"
#include "UIPlayerController.generated.h"

/**
 * 캐릭터가 없는 단순 UI 화면 일때만 사용하는 플레이어 컨트로러.
 * 대표적으로 트랜지션 맵이나 메인 화면.
 */
UCLASS()
class UIMODULE_API AUIPlayerController : public APlayerController, public IUIPlayerControllerInterface
{
	GENERATED_BODY()

public:
	/* begin IUIPlayerControllerInterface */
	virtual void SetTraveledMapPathToClient(const FString& MapPath) override;
	/* end IUIPlayerControllerInterface */
	
	/* 맵 로드가 완료될때 레벨 매니저 서브시스템에 알리는 서버함수 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Brandnew|Map", meta = (DisplayName ="Server Notify Map Loaded"))
	void Server_NotifyMapLoaded();


private:
	/* Map Asset Name을 클라이언트측의 Subsystem에 설정해야하기 때문에 RPC 사용 */
	UFUNCTION(Client, Reliable)
	void Client_SetTraveledMapPath(const FString& MapPath);
	
};
