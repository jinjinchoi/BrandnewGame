// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BnDialogueTextNode.h"
#include "BnDialogueSequenceNode.generated.h"

class ULevelSequence;
/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UBnDialogueSequenceNode : public UBnDialogueTextNode
{
	GENERATED_BODY()
public:
	TSoftObjectPtr<ULevelSequence> Sequence;
	
};
