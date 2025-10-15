// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/DialogueWidgetController.h"

#include "Manager/DialogueSubSystem.h"
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
	const UBnDialogueTextNode* TextNode = DialogueSubSystem->GetTextNodeById<UBnDialogueTextNode>(DialogueId);
	if (!TextNode) return;

	DialogueId = TextNode->NextNodeId;

	FTextDialogueParams DialogueParams;
	DialogueParams.Dialogue = TextNode->DialogueText;
	DialogueParams.SpeakerName = TextNode->SpeakerName;
	
	TextDialogueReceivedDelegate.Broadcast(DialogueParams);
	
}
