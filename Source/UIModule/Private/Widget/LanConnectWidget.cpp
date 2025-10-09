// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LanConnectWidget.h"

void ULanConnectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &ThisClass::HandleNetworkFailure);
	}
	
}

void ULanConnectWidget::NativeDestruct()
{
	if (GEngine)
	{
		GEngine->OnNetworkFailure().RemoveAll(this);
	}
	
	Super::NativeDestruct();
}

void ULanConnectWidget::OnConnectFailed_Implementation()
{
}

void ULanConnectWidget::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("Network failure: %s"), *ErrorString);

	OnConnectFailed();
	
}
