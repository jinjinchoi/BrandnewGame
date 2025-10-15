// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BnDialogueNodeBase.h"
#include "BnDialogueTextNode.generated.h"



/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UBnDialogueTextNode : public UBnDialogueNodeBase
{
	GENERATED_BODY()

public:
	FText SpeakerName;
	FText DialogueText;
	
	FName NextNodeId;
	
};
