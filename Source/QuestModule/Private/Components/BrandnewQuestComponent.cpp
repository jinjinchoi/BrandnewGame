// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestModule/Public/Components/BrandnewQuestComponent.h"

#include "Net/UnrealNetwork.h"


UBrandnewQuestComponent::UBrandnewQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	

}

void UBrandnewQuestComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ThisClass, ActivatedQuests, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, CompletedQuests, COND_OwnerOnly);
	
}

void UBrandnewQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateAllQuestMap();
	
}


void UBrandnewQuestComponent::CreateAllQuestMap()
{
	check(QuestDataTable);
	if (!QuestDataTable || !GetOwner()->HasAuthority() || !AllQuestsMap.IsEmpty()) return;
	
	if (QuestDataTable->GetRowStruct() == FQuestObjectiveBase::StaticStruct())
	{
		for (const TPair<FName, unsigned char*>& RowMap : QuestDataTable->GetRowMap())
		{
			const FQuestObjectiveBase* QuestObjective = reinterpret_cast<FQuestObjectiveBase*>(RowMap.Value);
			AllQuestsMap.Add(QuestObjective->QuestId, *QuestObjective);
		}
	}

	for (const TPair<FName, FQuestObjectiveBase>& Pair : AllQuestsMap)
	{
		FQuestObjectiveBase Quest = Pair.Value;
		LevelToQuestsMap.FindOrAdd(Quest.MinLevel).Add(Quest);
	}
}

void UBrandnewQuestComponent::GrantQuestByLevelRequirement(const int32 PlayerLevel)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	
	if (LevelToQuestsMap.IsEmpty())
	{
		CreateAllQuestMap();
	}

	for (int32 i = 1; i <= PlayerLevel; ++i)
	{
		if (!LevelToQuestsMap.Contains(PlayerLevel)) continue;

		for (const FQuestObjectiveBase QuestObjective : LevelToQuestsMap[i])
		{
			FQuestInstance NewQuest;
			NewQuest.QuestId = QuestObjective.QuestId;
			NewQuest.QuestObjective = QuestObjective;
			NewQuest.CurrentCount = 0;
			NewQuest.QuestState = EQuestState::InProgress;

			if (!ActivatedQuests.Contains(NewQuest) && !CompletedQuests.Contains(NewQuest))
			{
				ActivatedQuests.Add(NewQuest);
			}
		}
	}
}