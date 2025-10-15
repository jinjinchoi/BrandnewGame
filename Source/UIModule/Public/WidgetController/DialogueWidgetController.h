// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "DialogueWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FTextDialogueParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText SpeakerName = FText();

	UPROPERTY(BlueprintReadOnly)
	FText Dialogue = FText();
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextDialogueReceived , const FTextDialogueParams&, TextDialogueParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);
/**
 * 
 */
UCLASS()
class UIMODULE_API UDialogueWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BroadCastInitialValue() override;
	
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Dialogue Function")
	void BroadCastDialogue();
	
	UPROPERTY(BlueprintReadOnly, Category = "Brandnew|Dialogue")
	FName DialogueId;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnTextDialogueReceived TextDialogueReceivedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnDialogueEnded DialogueEndedDelegate;

private:
	void HandleTextDialogue();
	
};
