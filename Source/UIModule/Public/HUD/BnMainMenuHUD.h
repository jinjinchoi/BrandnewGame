// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BnMainMenuHUD.generated.h"

/**
 * 메인화면에서 사용하는 HUD
 */
UCLASS()
class UIMODULE_API ABnMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Widget")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;


private:
	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenuWidget;
	
};
