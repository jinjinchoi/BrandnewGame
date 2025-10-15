#pragma once

#include "Node/BnDialogueChoiceNode.h"
#include "LevelSequence.h"
#include "DialogueStructType.generated.h"

// 텍스트노드
USTRUCT(BlueprintType)
struct FTextDialogueDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName NodeId = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SpeakerName = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DialogueText = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName NextNodeId = NAME_None;
	
};

// 시퀀스 노드
USTRUCT(BlueprintType)
struct FSequenceDialogueDataRow : public FTextDialogueDataRow
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<ULevelSequence> Sequence = nullptr;
	
};

// 선택지 노드
USTRUCT(BlueprintType)
struct FChoiceDialogueDataRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 선택지를 여러개 보여줄 것이기 때문에 선택지를 그룹으로 묶어서 관리. 대화 노드에서 이 그룹 아이디로 선택지를 지정 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ChoiceGroupId = NAME_None;

	// 선택지 하나의 Id
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName NodeId = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogueChoiceType ChoiceType = EDialogueChoiceType::Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)

	FText ChoiceText = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName NextNodeId = FName();

	// 퀘스트 수락 또는 완료 선택지일 경우 어떤 퀘스트 인지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName QuestID = NAME_None;
	
};