// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFunctionLibrary.generated.h"

class UCharacterInfoWidgetController;
/**
 * 
 */
UCLASS()
class UIMODULE_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "UIFunctionLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UCharacterInfoWidgetController* GetCharacterInfoWidgetController(const UObject* WorldContextObject); 
	
};
