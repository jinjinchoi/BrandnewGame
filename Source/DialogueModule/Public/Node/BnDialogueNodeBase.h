// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueType/DialogueEnum.h"
#include "UObject/Object.h"
#include "BnDialogueNodeBase.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UBnDialogueNodeBase : public UObject
{
	GENERATED_BODY()

public:
	FName NodeId;
	EDialogueType NodeType;
	
};
