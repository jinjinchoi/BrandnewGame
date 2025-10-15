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

USTRUCT(BlueprintType)
struct FChoiceNodeParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName NextNodeName = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	FText ChoiceText = FText();

	UPROPERTY(BlueprintReadOnly)
	FName QuestID = NAME_None; 

	UPROPERTY(BlueprintReadOnly)
	bool bIsLastOption = false;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextDialogueReceived , const FTextDialogueParams&, TextDialogueParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceNodeReceived , const FChoiceNodeParams&, ChoiceNodeParams);
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

	UFUNCTION(BlueprintCallable, Category = "Brandnew|Dialogue Function")
	void ChoiceButtonClick(const FName& NextNodeId);
	
	UPROPERTY(BlueprintReadOnly, Category = "Brandnew|Dialogue")
	FName DialogueId;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnTextDialogueReceived TextDialogueReceivedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnChoiceNodeReceived ChoiceNodeReceivedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnDialogueEnded DialogueEndedDelegate;

private:
	void HandleTextDialogue();
	void HandleChoiceNode() const;
	
};
