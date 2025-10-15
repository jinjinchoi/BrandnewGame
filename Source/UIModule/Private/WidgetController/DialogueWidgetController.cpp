// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/DialogueWidgetController.h"

#include "Manager/DialogueSubSystem.h"
#include "Node/BnDialogueChoiceNode.h"
#include "Node/BnDialogueTextNode.h"

void UDialogueWidgetController::BroadCastInitialValue()
{
	BroadCastDialogue();
	
}

void UDialogueWidgetController::BroadCastDialogue()
{
	if (DialogueId.IsNone()) return;
	
	const UDialogueSubSystem* DialogueSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueSubSystem>();
	if (!DialogueSubSystem) return;
	
	switch (DialogueSubSystem->GetDialogueTypeById(DialogueId))
	{
	case EDialogueType::None:
		break;
		
	case EDialogueType::Text:
		HandleTextDialogue();
		break;
		
	case EDialogueType::Sequence:
		break;
		
	case EDialogueType::Choice:
		HandleChoiceNode();
		break;
		
	case EDialogueType::Condition:
		break;
		
	case EDialogueType::End:
		DialogueEndedDelegate.Broadcast();
		break;
	}
	
	
}

void UDialogueWidgetController::HandleTextDialogue()
{
	const UDialogueSubSystem* DialogueSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueSubSystem>();
	const UBnDialogueTextNode* TextNode = DialogueSubSystem->GetDialogueNodeById<UBnDialogueTextNode>(DialogueId);
	if (!TextNode) return;

	DialogueId = TextNode->NextNodeId;

	FTextDialogueParams DialogueParams;
	DialogueParams.Dialogue = TextNode->DialogueText;
	DialogueParams.SpeakerName = TextNode->SpeakerName;
	
	TextDialogueReceivedDelegate.Broadcast(DialogueParams);
	
}

void UDialogueWidgetController::HandleChoiceNode() const
{
	const UDialogueSubSystem* DialogueSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UDialogueSubSystem>();
	const UBnDialogueChoiceNode* ChoiceNode = DialogueSubSystem->GetDialogueNodeById<UBnDialogueChoiceNode>(DialogueId);
	if (!ChoiceNode) return;
	
	for (int32 i = 0; i < ChoiceNode->DialogueChoices.Num(); ++i)
	{
		const FChoiceNodeInfo& ChoiceNodeInfo = ChoiceNode->DialogueChoices[i];

		FChoiceNodeParams ChoiceNodeParams;
		ChoiceNodeParams.ChoiceText = ChoiceNodeInfo.ChoiceText;
		ChoiceNodeParams.NextNodeName = ChoiceNodeInfo.NextNodeId;
		ChoiceNodeParams.bIsLastOption = i == ChoiceNode->DialogueChoices.Num() - 1;
		
		ChoiceNodeReceivedDelegate.Broadcast(ChoiceNodeParams);
		

		// if (ChoiceNodeInfo.ChoiceType > EDialogueChoiceType::Normal)
		// {
		//		TODO: 퀘스트 노드인 경우 처리해야함
		// }
		
	}
	
}


void UDialogueWidgetController::ChoiceButtonClick(const FName& NextNodeId)
{
	DialogueId = NextNodeId;
	HandleTextDialogue();
}
