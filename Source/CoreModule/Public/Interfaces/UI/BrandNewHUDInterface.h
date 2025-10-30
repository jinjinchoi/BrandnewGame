// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BrandNewHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBrandNewHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBrandNewHUDInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RequestInitHUD() = 0;
	virtual void CreateDialogueWidget(const FName& FirstDialogueId) = 0;
	virtual void HideMainOverlay() = 0;
	virtual void CreateSequenceOverlayWidget() = 0;
	virtual void RemoveSequenceOverlayWidget() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Brandnew|Interface Function")
	void ShowMainOverlay();
	
};
