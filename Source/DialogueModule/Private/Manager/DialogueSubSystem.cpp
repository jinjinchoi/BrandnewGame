// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DialogueSubSystem.h"

#include "Graph/BnDialogueGraph.h"

void UDialogueSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	DialogueGraph = NewObject<UBnDialogueGraph>(this);
	DialogueGraph->Init();
	
}
