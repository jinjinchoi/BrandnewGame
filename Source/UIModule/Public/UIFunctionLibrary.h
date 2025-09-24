// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFunctionLibrary.generated.h"

class UCharacterInfoWidgetController;

USTRUCT(BlueprintType)
struct FSaveSlotViewInfoParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText TitleText = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	FText Date = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	int32 Level = 1;

	UPROPERTY(BlueprintReadOnly)
	FText LocationName = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;	
	
};

/**
 * 
 */
UCLASS()
class UIMODULE_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "UIFunctionLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UCharacterInfoWidgetController* GetCharacterInfoWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "UIFunctionLibrary|SaveLogic")
	static FSaveSlotViewInfoParams GetSaveSlotInfo(const FString& SlotName, const int32 SlotIndex = 1);

	UFUNCTION(BlueprintCallable, Category = "UIFunctionLibrary|SaveLogic")
	static void RequestSave(ACharacter* PlayerCharacter, const FString& SlotName, const int32 SlotIndex = 1);
	
};
