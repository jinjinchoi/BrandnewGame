// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController/BrandNewWidgetControllerBase.h"
#include "GameOverWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class UIMODULE_API UGameOverWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Character")
	void ReviveCharacter() const;
	
};
