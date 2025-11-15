#pragma once

#include "QuestEnum.h"

#include "QuestType.generated.h"


/* 퀘스트 설정하는 데이터 테이블을 위한 구조체 */
USTRUCT(BlueprintType)
struct FQuestObjectiveBase : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Info")
	FName QuestId = NAME_None;
	
	/* 퀘스트를 수행하기 위해 필요한 레벨. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Info")
	int32 MinLevel = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Dialogue")
	FName DialogueId = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|UI")
	EQuestType QuestType = EQuestType::MainQuest;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|UI")
	FText QuestTitle = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|UI")
	FText QuestObjectText = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|UI")
	FText QuestDescription = FText();
	
	/* 퀘스트의 목표가 되는 아이디로 퀘스트 타겟은 플레이어에게 자신의 아이디를 보내 타켓 카운터를 올려 퀘스트 완료 여부를 판단함 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Objective")
	FName TargetId = NAME_None;

	// 퀘스트를 완료하기 위해 필요한 목표 수로 0이면 즉시 완료되는 퀘스트(대표적으로 NPC와 대화나 특정 지역 도달)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Objective")
	int32 TargetCount = 0;
	
	/* 퀘스트를 완료할 타겟의 Id */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Objective")
	FName QuestCompletionTargetId = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Reward")
	int32 RewardXp = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Reward")
	TArray<FName> RewardItemIds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestTable|Info")
	FName NextQuestId = NAME_None;
	
};