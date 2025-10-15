// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/BnDialogueGraph.h"

#include "DialogueType/DialogueStructType.h"
#include "Game/GameInstance/BrandNewGameInstance.h"
#include "Node/BnDialogueSequenceNode.h"
#include "Node/BnDialogueTextNode.h"

void UBnDialogueGraph::Init()
{
	CreateTextNode();
	CreateSequenceNode();
	CreateChoiceNode();
}

UBnDialogueNodeBase* UBnDialogueGraph::GetNodeFromId(const FName& NodeId) const
{
	if (NodeMap.Find(NodeId))
	{
		return *NodeMap.Find(NodeId);
	}

	return nullptr;
}

void UBnDialogueGraph::CreateTextNode()
{
	const UBrandNewGameInstance* BrandNewGameInstance =  GetWorld()->GetGameInstance<UBrandNewGameInstance>();
	if (!BrandNewGameInstance || !BrandNewGameInstance->GetTextDialogueDataTable()) return;

	if (BrandNewGameInstance->GetTextDialogueDataTable()->GetRowStruct() == FTextDialogueDataRow::StaticStruct())
	{
		for (const TPair<FName, unsigned char*>& RowMap : BrandNewGameInstance->GetTextDialogueDataTable()->GetRowMap())
		{
			const FTextDialogueDataRow* TextData = reinterpret_cast<FTextDialogueDataRow*>(RowMap.Value);
			UBnDialogueTextNode* TextNode = NewObject<UBnDialogueTextNode>();
			TextNode->NodeId = TextData->NodeId;
			TextNode->NodeType = EDialogueType::Text;
			
			TextNode->DialogueText = TextData->DialogueText;
			TextNode->NextNodeId = TextData->NextNodeId;
			TextNode->SpeakerName = TextData->SpeakerName;

			NodeMap.Add(TextData->NodeId, TextNode);
		}
	}
}

void UBnDialogueGraph::CreateSequenceNode()
{
	const UBrandNewGameInstance* BrandNewGameInstance =  GetWorld()->GetGameInstance<UBrandNewGameInstance>();
	if (!BrandNewGameInstance || !BrandNewGameInstance->GetSequenceDialogueDataTable()) return;

	if (BrandNewGameInstance->GetSequenceDialogueDataTable()->GetRowStruct() == FSequenceDialogueDataRow::StaticStruct())
	{
		for (const TPair<FName, unsigned char*>& RowMap : BrandNewGameInstance->GetSequenceDialogueDataTable()->GetRowMap())
		{
			const FSequenceDialogueDataRow* SequenceDialogueData = reinterpret_cast<FSequenceDialogueDataRow*>(RowMap.Value);
			
			UBnDialogueSequenceNode* SequenceNode = NewObject<UBnDialogueSequenceNode>();
			SequenceNode->NodeId = SequenceDialogueData->NodeId;
			SequenceNode->NodeType = EDialogueType::Sequence;
			
			SequenceNode->DialogueText = SequenceDialogueData->DialogueText;
			SequenceNode->NextNodeId = SequenceDialogueData->NextNodeId;
			SequenceNode->SpeakerName = SequenceDialogueData->SpeakerName;
			SequenceNode->Sequence = SequenceDialogueData->Sequence;

			NodeMap.Add(SequenceDialogueData->NodeId, SequenceNode);
		}
	}
	
}

void UBnDialogueGraph::CreateChoiceNode()
{
	const UBrandNewGameInstance* BrandNewGameInstance =  GetWorld()->GetGameInstance<UBrandNewGameInstance>();
	if (!BrandNewGameInstance || !BrandNewGameInstance->GetChoiceDialogueDataTable()) return;

	if (BrandNewGameInstance->GetChoiceDialogueDataTable()->GetRowStruct() == FChoiceDialogueDataRow::StaticStruct())
	{
		for (const TPair<FName, unsigned char*>& RowMap : BrandNewGameInstance->GetSequenceDialogueDataTable()->GetRowMap())
		{
			const FChoiceDialogueDataRow* ChoiceDialogueData = reinterpret_cast<FChoiceDialogueDataRow*>(RowMap.Value);

			// 선택지의 정보를 설정
			FChoiceNodeInfo ChoiceNodeInfo;
			ChoiceNodeInfo.ChoiceNodeId = ChoiceDialogueData->NodeId; // 선택지 하나의 Id를 설정
			ChoiceNodeInfo.NextNodeId = ChoiceDialogueData->NextNodeId;
			ChoiceNodeInfo.ChoiceText = ChoiceDialogueData->ChoiceText;
			ChoiceNodeInfo.ChoiceType = ChoiceDialogueData->ChoiceType;
			ChoiceNodeInfo.QuestID = ChoiceDialogueData->QuestID;

			// 데이터 테이블에서 직접 배열을 Import 해올 수 없어 그룹으로 선택지 노드들을 묶고 이를 배열로 만드는 작업 실시
			if (UBnDialogueNodeBase** ExistedNodePtr = NodeMap.Find(ChoiceDialogueData->ChoiceGroupId))
			{
				if (UBnDialogueChoiceNode* ChoiceNode = Cast<UBnDialogueChoiceNode>(*ExistedNodePtr))
				{
					// 이미 선택지 배열이 존재하면 배열에 추가 선택지를 배열에 추가
					ChoiceNode->DialogueChoices.Add(ChoiceNodeInfo);
				}
			}
			else // 배열이 아직 존재하지 않으면 새로 만들어서 추가.
			{
				UBnDialogueChoiceNode* ChoiceNode = NewObject<UBnDialogueChoiceNode>();
				ChoiceNode->NodeId = ChoiceDialogueData->ChoiceGroupId; // 선택지 노드 자체의 아이디는 그룹 아이디로 설정
				ChoiceNode->NodeType = EDialogueType::Choice;
				ChoiceNode->DialogueChoices.Add(ChoiceNodeInfo);
				NodeMap.Add(ChoiceDialogueData->ChoiceGroupId, ChoiceNode);
			}
		}
	}
	
}

