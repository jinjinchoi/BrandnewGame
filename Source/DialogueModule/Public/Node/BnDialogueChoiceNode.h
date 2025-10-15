// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BnDialogueNodeBase.h"
#include "BnDialogueChoiceNode.generated.h"


UENUM(BlueprintType)
enum class EDialogueChoiceType : uint8
{
	Normal,     // 일반 선택지
	QuestOffer, // 퀘스트 수락 선택지
	QuestReward // 퀘스트 완료/보상 선택지
};

USTRUCT(BlueprintType)
struct FChoiceNodeInfo
{
	GENERATED_BODY()

	// 초이스 개별 노드의 아이디. 부모 클래스에 존재하는 노드 아이디는 그룹 아이디로 지정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ChoiceNodeId = NAME_None;

	// 선택지 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDialogueChoiceType ChoiceType = EDialogueChoiceType::Normal;
	
	UPROPERTY(BlueprintReadOnly)
	FText ChoiceText = FText();

	UPROPERTY(BlueprintReadOnly)
	FName NextNodeId = FName();

	// 퀘스트 수락 또는 완료 선택지 일 경우 관련된 Quest의 Id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="ChoiceType != EDialogueChoiceType::Normal"))
	FName QuestID; 
	
};

/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UBnDialogueChoiceNode : public UBnDialogueNodeBase
{
	GENERATED_BODY()

public:
	TArray<FChoiceNodeInfo> DialogueChoices;
	
};
