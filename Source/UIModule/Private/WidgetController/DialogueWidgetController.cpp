// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/DialogueWidgetController.h"

#include "Game/GameInstance/BrandNewGameInstance.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "Manager/DialogueSubSystem.h"
#include "Manager/Sequnce/SequenceManager.h"
#include "Node/BnDialogueChoiceNode.h"
#include "Node/BnDialogueSequenceNode.h"
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
		HandleTextNode();
		break;
		
	case EDialogueType::Sequence:
		HandleSequenceNode();
		break;
		
	case EDialogueType::Choice:
		HandleChoiceNode();
		break;
		
	case EDialogueType::Condition:
		break;
		
	case EDialogueType::End:
		HandleEndNode();
		break;
	}
	
	
}

void UDialogueWidgetController::HandleTextNode()
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

void UDialogueWidgetController::HandleSequenceNode()
{
	const UBrandNewGameInstance* BnGameInstance = GetWorld()->GetGameInstance<UBrandNewGameInstance>();
	const UDialogueSubSystem* DialogueSubSystem = BnGameInstance->GetSubsystem<UDialogueSubSystem>();
	const UBnDialogueSequenceNode* SequenceNode = DialogueSubSystem->GetDialogueNodeById<UBnDialogueSequenceNode>(DialogueId);
	USequenceManager* SequenceManager = BnGameInstance->GetSequenceManager();
	if (!SequenceNode) return;

	DialogueId = SequenceNode->NextNodeId;
	
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn))
	{
		PlayerInterface->SetCombatWeaponVisible(false);
	}

	FTextDialogueParams DialogueParams;
	DialogueParams.Dialogue = SequenceNode->DialogueText;
	DialogueParams.SpeakerName = SequenceNode->SpeakerName;
	SequenceManager->PlayDialogueSequence(SequenceNode->Sequence);

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
	HandleTextNode();
}


void UDialogueWidgetController::HandleEndNode() const
{
	DialogueEndedDelegate.Broadcast();
	
	// Note: 마지막 노드가 시퀀스 노드였는지 확인하고 작업들 진행하게 바꿀 수도 있음
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(ControlledPawn))
	{
		PlayerInterface->SetCombatWeaponVisible(true);
	}
	
	const UBrandNewGameInstance* BnGameInstance = GetWorld()->GetGameInstance<UBrandNewGameInstance>();
	USequenceManager* SequenceManager = BnGameInstance->GetSequenceManager();
	SequenceManager->StopCurrentSequence();
	
}