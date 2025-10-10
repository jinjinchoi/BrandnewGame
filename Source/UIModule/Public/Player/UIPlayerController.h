// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UIMODULE_API AUIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/* 맵 로드가 완료될때 서버에서 게임스테이트에 알려주는 함수 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Map", meta = (DisplayName ="Notify Map Loaded"))
	void K2_NotifyMapLoaded();

private:
	UFUNCTION(Server, Reliable)
	void Server_NotifyMapLoaded();
	void NotifyMapLoaded() const;
	
};
