// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DialogueSubSystem.h"

#include "Graph/BnDialogueGraph.h"

void UDialogueSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DialogueGraph = NewObject<UBnDialogueGraph>(this);
	DialogueGraph->Init();
	
}

EDialogueType UDialogueSubSystem::GetDialogueTypeById(const FName& DialogueId) const
{
	if (const UBnDialogueNodeBase* DialogueNodeBase = DialogueGraph->GetNodeFromId(DialogueId))
	{
		return DialogueNodeBase->NodeType;
	}

	return EDialogueType::None;
}
