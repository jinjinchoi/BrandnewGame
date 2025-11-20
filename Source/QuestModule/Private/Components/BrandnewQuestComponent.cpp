// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestModule/Public/Components/BrandnewQuestComponent.h"

#include "Game/Subsystem/BrandnewQuestSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Actor/QuestActorInterface.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
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
			AddActivatedQuest(QuestObjective);
		}
	}
}

void UBrandnewQuestComponent::GrantQuestByQuestId(const FName& QuestId)
{
	if (ActivatedQuests.Contains(QuestId) || CompletedQuests.Contains(QuestId)) return;
	
	if (const FQuestObjectiveBase* Found = AllQuestsMap.Find(QuestId))
	{
		const FQuestObjectiveBase QuestObjective = *Found;
		AddActivatedQuest(QuestObjective);
	}
}

void UBrandnewQuestComponent::RestoreQuestProgress(const TArray<FQuestProgress>& QuestProgresses)
{
	if (LevelToQuestsMap.IsEmpty())
	{
		CreateAllQuestMap();
	}
	
	for (const FQuestProgress& QuestProgress : QuestProgresses)
	{
		const FQuestObjectiveBase QuestObjective = FindQuestObjectiveById(QuestProgress.QuestId);
		
		FQuestInstance QuestToRestore;
		QuestToRestore.QuestId = QuestProgress.QuestId;
		QuestToRestore.CurrentCount = QuestProgress.Progress;
		
		QuestToRestore.TargetCount = QuestObjective.TargetCount;
		QuestToRestore.TargetId = QuestObjective.TargetId;
		QuestToRestore.NextQuestId = QuestObjective.NextQuestId;
		QuestToRestore.DialogueId = QuestObjective.DialogueId;
		
		if (!ActivatedQuests.Contains(QuestToRestore) && !CompletedQuests.Contains(QuestToRestore))
		{
			ActivatedQuests.Add(QuestToRestore);
		}
	}
	
}

void UBrandnewQuestComponent::RestoreCompletedQuests(const TArray<FName>& CompletedQuestsToRestore)
{
	if (LevelToQuestsMap.IsEmpty())
	{
		CreateAllQuestMap();
	}
	
	for (const FName& CompletedQuestId : CompletedQuestsToRestore)
	{
		const FQuestObjectiveBase QuestObjective = FindQuestObjectiveById(CompletedQuestId);
		
		FQuestInstance QuestToRestore;
		QuestToRestore.QuestId = CompletedQuestId;
		QuestToRestore.TargetCount = QuestObjective.TargetCount;
		QuestToRestore.CurrentCount = QuestObjective.TargetCount;
		QuestToRestore.TargetId = QuestObjective.TargetId;
		QuestToRestore.NextQuestId = QuestObjective.NextQuestId;
		QuestToRestore.DialogueId = QuestObjective.DialogueId;
		
		if (!ActivatedQuests.Contains(QuestToRestore) && !CompletedQuests.Contains(QuestToRestore))
		{
			CompletedQuests.Add(QuestToRestore);
		}
	}
	
}

void UBrandnewQuestComponent::AddActivatedQuest(const FQuestObjectiveBase& QuestObjective)
{
	FQuestInstance NewQuest;
	NewQuest.QuestId = QuestObjective.QuestId;
	NewQuest.TargetCount = QuestObjective.TargetCount;
	NewQuest.CurrentCount = 0;
	NewQuest.TargetId = QuestObjective.TargetId;
	NewQuest.NextQuestId = QuestObjective.NextQuestId;
	NewQuest.DialogueId = QuestObjective.DialogueId;
			
	if (!ActivatedQuests.Contains(NewQuest) && !CompletedQuests.Contains(NewQuest))
	{
		if (ActivatedQuests.IsEmpty())
		{
			SetTrackedQuestId(NewQuest.QuestId);
		}
		ActivatedQuests.Add(NewQuest);
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

FQuestObjectiveBase UBrandnewQuestComponent::FindQuestObjectiveById(const FName& QuestIdToFind) const
{
	if (const FQuestObjectiveBase* Found = AllQuestsMap.Find(QuestIdToFind))
	{
		return *Found;
	}

	return FQuestObjectiveBase();
	
}

FQuestObjectiveBase UBrandnewQuestComponent::FindTrackedQuestObjective() const
{
	if (const FQuestObjectiveBase* Found = AllQuestsMap.Find(TrackedQuestId))
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

TArray<FQuestProgress> UBrandnewQuestComponent::GetQuestProgress() const
{
	TArray<FQuestProgress> QuestProgressArray;
	
	for (const FQuestInstance& Quest : ActivatedQuests)
	{
		FQuestProgress Progress (Quest.QuestId, Quest.CurrentCount);
		QuestProgressArray.Add(Progress);
	}
	
	return QuestProgressArray;
	
}

TArray<FName> UBrandnewQuestComponent::CompletedQuestIds() const
{
	TArray<FName> CompletedQuestIds;
	for (const FQuestInstance& Quest : CompletedQuests)
	{
		CompletedQuestIds.Add(Quest.QuestId);
	}
	
	return CompletedQuestIds;
	
}

void UBrandnewQuestComponent::IncreaseQuestProgress(const FName& QuestIdToUpdate, const int32 IncreaseAmount)
{
	if (!GetOwner()->HasAuthority()) return;
	
	// 활성화된 퀘스트 순회하여 매개변수로 받은 Id를 필요로 하는 퀘스트가 있는지 확인
	for (FQuestInstance& Quest : ActivatedQuests)
	{
		if (Quest.QuestId == QuestIdToUpdate)
		{
			// 퀘스트 찾으면 카운터 증가
			Quest.CurrentCount = FMath::Clamp(Quest.CurrentCount + IncreaseAmount, 0, Quest.TargetCount);
			if (Quest.CurrentCount >= Quest.TargetCount)
			{
				CompleteQuest(Quest.QuestId);
				return;
			}
		}
	}
	
	if (QuestIdToUpdate == TrackedQuestId)
	{
		OnTrackedQuestChangedDelegate.Broadcast();
	}
	
}


void UBrandnewQuestComponent::CompleteQuest(const FName& CompletedQuestId)
{
	// 클리어한 퀘스트의 아이디를 바탕으로 인덱스를 찾아옴
	const int32 Index = ActivatedQuests.IndexOfByPredicate(
		[&](const FQuestInstance& Quest)
		{
			return Quest.QuestId == CompletedQuestId;
		}
	);

	if (Index == INDEX_NONE) return;
	
	// 클리어한 퀘스트
	const FQuestInstance CompletedQuest = ActivatedQuests[Index];
	
	// 클리어한 퀘스트가 연속 퀘스트일 경우 새로운 퀘스트 부여
	if (!CompletedQuest.NextQuestId.IsNone())
	{
		GrantQuestByQuestId(CompletedQuest.NextQuestId);
	}
	// 클리어한 퀘스트가 현재 추적중이었으면 추적 대상을 다음 퀘스트로 변경 
	if (CompletedQuest.QuestId == TrackedQuestId)
	{
		SetTrackedQuestId(CompletedQuest.NextQuestId);
	}
	// 현재 TrackedQuestId는 복제가 안되서 if 안에서 실행 안하고 클라이언트가 따로 확인
	Client_SetTrackedQuestId(CompletedQuest.QuestId, CompletedQuest.NextQuestId);
	
	// 보상 추가
	GrantQuestRewardsToPlayer(CompletedQuest.QuestId);
	
	// Activated에서 제거
	ActivatedQuests.RemoveAt(Index);

	// Completed로 이동
	CompletedQuests.Add(CompletedQuest);
	
}

void UBrandnewQuestComponent::GrantQuestRewardsToPlayer(const FName& QuestId) const
{
	const FQuestObjectiveBase& QuestObjectiveBase = FindQuestObjectiveById(QuestId);
	if (QuestObjectiveBase.QuestId == NAME_None) return;
	
	const APlayerState* PlayerState = CastChecked<APlayerState>(GetOwner());
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(PlayerState->GetPawn()))
	{
		PlayerInterface->GrantQuestReward(QuestObjectiveBase.RewardXp, QuestObjectiveBase.RewardItemMap);
	}
}

void UBrandnewQuestComponent::OnRep_ActivatedQuests()
{
	SetTrackedQuestId(TrackedQuestId);
}

void UBrandnewQuestComponent::Client_SetTrackedQuestId_Implementation(const FName& CompletedQuestId, const FName& NextQuestId)
{
	if (CompletedQuestId == TrackedQuestId)
	{
		SetTrackedQuestId(NextQuestId);
	}
}
