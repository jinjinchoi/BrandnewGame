// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestModule/Public/Components/BrandnewQuestComponent.h"

#include "Game/Subsystem/BrandnewQuestSubsystem.h"
#include "Interfaces/Actor/QuestActorInterface.h"
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
	if (!QuestDataTable || !AllQuestsMap.IsEmpty()) return;
	
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
			if (QuestObjective.MinLevel <= 0) continue;
			
			FQuestInstance NewQuest;
			NewQuest.QuestId = QuestObjective.QuestId;
			NewQuest.TargetCount = QuestObjective.TargetCount;
			NewQuest.CurrentCount = 0;
			NewQuest.QuestState = EQuestState::InProgress;
			NewQuest.TargetId = QuestObjective.TargetId;

			if (!ActivatedQuests.Contains(NewQuest) && !CompletedQuests.Contains(NewQuest))
			{
				ActivatedQuests.Add(NewQuest);
			}
		}
	}
}


void UBrandnewQuestComponent::SetTrackedQuestId(const FName& QuestIdToTrack)
{
	UBrandnewQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UBrandnewQuestSubsystem>();
	
	// 이전에 화면에 표시 중이던 퀘스트 타겟 액터의 위젯 컴포넌트 hide
	if (TrackedQuestId != QuestIdToTrack)
	{
		const FQuestInstance PreviousTrackedQuestInstance = FindTrackedQuestInstance();
		AActor* PreviousTargetActor = QuestSubsystem->GetActorFromMap(PreviousTrackedQuestInstance.TargetId);
		if (IQuestActorInterface* PreviousQuestActorInterface = Cast<IQuestActorInterface>(PreviousTargetActor))
		{
			PreviousQuestActorInterface->ShowLocationWidget(false);
		}
	}
	
	TrackedQuestId = QuestIdToTrack;
	OnTrackedQuestChangedDelegate.Broadcast();
	
	if (TrackedQuestId == NAME_None) return;
	
	// 새로 바뀐 타겟의 위젯 컴포넌트 show
	const FQuestInstance TrackedQuestInstance = FindTrackedQuestInstance();
	AActor* TargetActor = QuestSubsystem->GetActorFromMap(TrackedQuestInstance.TargetId);
	if (IQuestActorInterface* QuestActorInterface = Cast<IQuestActorInterface>(TargetActor))
	{
		QuestActorInterface->ShowLocationWidget(true);
	}
	
	
	
}

FQuestObjectiveBase UBrandnewQuestComponent::FindQuestObjectiveById(const FName QuestIdToFind) const
{
	if (const FQuestObjectiveBase* Found = AllQuestsMap.Find(QuestIdToFind))
	{
		return *Found;
	}

	return FQuestObjectiveBase();
	
}

FQuestInstance UBrandnewQuestComponent::FindTrackedQuestInstance() const
{
	if (TrackedQuestId == NAME_None) return FQuestInstance();
	
	for (const FQuestInstance& Quest : ActivatedQuests)
	{
		if (Quest.QuestId == TrackedQuestId)
		{
			return Quest;
		}
	}
	
	return FQuestInstance();
	
}
