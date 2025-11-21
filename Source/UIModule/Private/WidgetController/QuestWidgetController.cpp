// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/QuestWidgetController.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "GameFramework/PlayerState.h"

UBrandnewQuestComponent* UQuestWidgetController::GetQuestComponent() const
{
	check(ControlledPawn)
	
	if (const IBnPlayerStateInterface* PlayerStateInterface = CastChecked<IBnPlayerStateInterface>(ControlledPawn->GetPlayerState()))
	{
		return Cast<UBrandnewQuestComponent>(PlayerStateInterface->GetQuestComponent());
	}
	
	return nullptr;

}

void UQuestWidgetController::BindCallbacksToDependencies() const
{
	UBrandnewQuestComponent* QuestComponent = GetQuestComponent();
	if (!QuestComponent) return;
	
	QuestComponent->OnTrackedQuestChangedDelegate.AddLambda([this](const FName& TrackedQuestId)
	{
		OnTrackedQuestChangedDelegate.Broadcast(TrackedQuestId);
	});
	
}

TArray<FQuestInstance> UQuestWidgetController::GetActivatedQuests() const
{
	if (const UBrandnewQuestComponent* QuestComponent = GetQuestComponent())
	{
		return QuestComponent->GetActivatedQuests();
	}
	
	return TArray<FQuestInstance>();
	
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
	if (const UBrandnewQuestComponent* QuestComponent = GetQuestComponent())
	{
		return QuestComponent->FindQuestObjectiveById(QuestId);
	}
	
	return FQuestObjectiveBase();

}

FQuestInstance UQuestWidgetController::FindTrackedQuestInstance() const
{
	if (const UBrandnewQuestComponent* QuestComponent = GetQuestComponent())
	{
		return GetQuestInstanceById(QuestComponent->GetTrackedQuestId());
	}
	
	return FQuestInstance();
	
}

FName UQuestWidgetController::GetTrackedQuestId() const
{
	if (const UBrandnewQuestComponent* QuestComponent = GetQuestComponent())
	{
		return QuestComponent->GetTrackedQuestId();
	}

	return FName();
	
}

void UQuestWidgetController::SetTrackedQuestId(const FName& QuestId)
{
	if (UBrandnewQuestComponent* QuestComponent = GetQuestComponent())
	{
		QuestComponent->SetTrackedQuestId(QuestId);
	}
}


