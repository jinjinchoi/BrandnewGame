// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BnDialogueNodeBase.h"
#include "BnDialogueConditionNode.generated.h"

/* 퀘스트 진행 여부 */
enum class EDialogueConditionType : uint8
{
	None,
	QuestNotStarted,
	QuestActive,
	QuestCompleted,
	Custom
};


/**
 * 퀘스트 아이디를 기반으로 현재 어떤 컨디션에 있는지 확인하고
 * 해당 컨디션에 맞으면 NextNodeId로 넘어감.
 * 보통 선택지를 보여줘야하기 때문에 Choice 노드로 갈 가능성이 높음.
 */
struct FDialogueCondition
{
	FName QuestID; 
	EDialogueConditionType ConditionType = EDialogueConditionType::None;
	FName NextNodeId = FName();
	
};

/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UBnDialogueConditionNode : public UBnDialogueNodeBase
{
	GENERATED_BODY()

public:
	TArray<FDialogueCondition> Conditions;
	
	
};
