// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/BnDialogueGraph.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Node/BnDialogueNodeBase.h"
#include "DialogueSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UDialogueSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	template<class T>
	T* GetDialogueNodeById(const FName& DialogueId) const;
	UBnDialogueNodeBase* GetDialogueNodeById(const FName& DialogueId) const;

	EDialogueType GetDialogueTypeById(const FName& DialogueId) const;
	
private:
	UPROPERTY()
	TObjectPtr<UBnDialogueGraph> DialogueGraph;
	
};

template <class T>
T* UDialogueSubSystem::GetDialogueNodeById(const FName& DialogueId) const
{
	static_assert(TIsDerivedFrom<T, UBnDialogueNodeBase>::IsDerived, "T must be derived from UBnDialogueNodeBase");

	return Cast<T>(DialogueGraph->GetNodeFromId(DialogueId));
	
}
