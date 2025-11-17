// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/QuestWidgetController.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "GameFramework/PlayerState.h"

TArray<FQuestInstance> UQuestWidgetController::GetActivatedQuests() const
{
	check(ControlledPawn)
	if (!ControlledPawn) return TArray<FQuestInstance>();
	
	const IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(ControlledPawn->GetPlayerState());
	if (!PlayerStateInterface) return TArray<FQuestInstance>();
	
	const UBrandnewQuestComponent* QuestComponent = Cast<UBrandnewQuestComponent>(PlayerStateInterface->GetQuestComponent());
	if (!QuestComponent) return TArray<FQuestInstance>();
	
	return QuestComponent->GetActivatedQuests();
	
}

FQuestInstance UQuestWidgetController::GetQuestInstanceById(const FName& QuestId) const
{
	TArray<FQuestInstance> QuestInstances = GetActivatedQuests();
	if (QuestInstances.IsEmpty()) return FQuestInstance();
	
	for (const FQuestInstance& QuestInstance : QuestInstances)
	{
		if (QuestInstance.QuestId == QuestId) return QuestInstance;
	}
	
	return FQuestInstance();
	
}

FQuestObjectiveBase UQuestWidgetController::FindQuestObjectiveById(const FName& QuestId) const
{
	check(ControlledPawn)
	if (!ControlledPawn) return FQuestObjectiveBase();

	const IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(ControlledPawn->GetPlayerState());
	if (!PlayerStateInterface) return FQuestObjectiveBase();

	const UBrandnewQuestComponent* QuestComponent = Cast<UBrandnewQuestComponent>(PlayerStateInterface->GetQuestComponent());
	if (!QuestComponent) return FQuestObjectiveBase();

	return QuestComponent->FindQuestObjectiveById(QuestId);
}
