// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BnDialogueGraph.generated.h"

class UBnDialogueNodeBase;

/**
 * 노드들을 저장하는 클래스
 */
UCLASS()
class DIALOGUEMODULE_API UBnDialogueGraph : public UObject
{
	GENERATED_BODY()

public:
	void Init();
	UBnDialogueNodeBase* GetNodeFromId(const FName& NodeId) const;

private:
	void CreateTextNode();
	void CreateSequenceNode();
	void CreateChoiceNode();
	void CreateEndNode();
	
	UPROPERTY()
	TMap<FName /* Node ID*/, UBnDialogueNodeBase* /* Node */ > NodeMap;
	
};
