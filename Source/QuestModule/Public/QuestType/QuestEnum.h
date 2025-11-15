#pragma once

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	InProgress, // 퀘스트 진행 중
	Completed // 퀘스트 완료
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainQuest,
	SubQuest
	
};