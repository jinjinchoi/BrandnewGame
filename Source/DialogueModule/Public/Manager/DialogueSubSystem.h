// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueSubSystem.generated.h"

class UBnDialogueGraph;
/**
 * 
 */
UCLASS()
class DIALOGUEMODULE_API UDialogueSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY()
	TObjectPtr<UBnDialogueGraph> DialogueGraph;
	
};
