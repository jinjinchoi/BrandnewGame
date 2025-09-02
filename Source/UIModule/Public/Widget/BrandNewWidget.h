// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/BnWidgetInterface.h"
#include "BrandNewWidget.generated.h"

/**
 * 
 */
UCLASS()
class UIMODULE_API UBrandNewWidget : public UUserWidget, public IBnWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	virtual void SetUIWidgetController(UObject* ControllerToSet) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
	
};
