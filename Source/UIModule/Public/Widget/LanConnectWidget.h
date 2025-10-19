// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LanConnectWidget.generated.h"

/**
 * 렌 연결할때 실패시 함수 호출하는 위젯
 */
UCLASS()
class UIMODULE_API ULanConnectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnConnectFailed();
	
private:
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	
};
