// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BrandNewHUD.generated.h"

class UBrandNewWidget;
/**
 * 
 */
UCLASS()
class UIMODULE_API ABrandNewHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitHUD();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Overlay")
	TSubclassOf<UBrandNewWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UBrandNewWidget> OverlayWidget;
	
};
