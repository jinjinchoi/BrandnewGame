// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestType/QuestType.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "BrandnewQuestComponent.generated.h"

class UBnQuestInstance;

/* 퀘스트 진행 상황등이 담기는 구조체 */
USTRUCT(BlueprintType)
struct FQuestInstance
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	FName QuestId = NAME_None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	int32 CurrentCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	FName TargetId = NAME_None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	int32 TargetCount = 0;
	
	UPROPERTY()
	FName DialogueId = NAME_None;
	
	UPROPERTY()
	FName NextQuestId = NAME_None;
	
	bool operator==(const FQuestInstance& Other) const
	{
		return QuestId == Other.QuestId;
	}
	
	bool operator==(const FName& OtherQuestId) const
	{
		return QuestId == OtherQuestId;
	}
};

DECLARE_MULTICAST_DELEGATE(FOnTrackedQuestChanged);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTMODULE_API UBrandnewQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBrandnewQuestComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* 레벨을 보내면 해당 레벨에 수행 가능한 퀘스트를 추가해주는 함수 */
	void GrantQuestByLevelRequirement(const int32 PlayerLevel);
	void GrantQuestByQuestId(const FName& QuestId);
	
	void RestoreQuestProgress(const TArray<FQuestProgress>& QuestProgresses);
	void RestoreCompletedQuests(const TArray<FName>& CompletedQuestsToRestore);
	
	FQuestObjectiveBase FindQuestObjectiveById(const FName& QuestIdToFind) const;
	FQuestObjectiveBase FindTrackedQuestObjective() const;
	FQuestInstance FindTrackedQuestInstance() const;
	
	TArray<FQuestProgress> GetQuestProgress() const;
	TArray<FName> CompletedQuestIds() const;

	// 현재 추적중인 퀘스트 아이디 설정하는 함수
	void SetTrackedQuestId(const FName& QuestIdToTrack);
	// 추적 중인 퀘스트가 변경되는 호출되는 델리게이트
	FOnTrackedQuestChanged OnTrackedQuestChangedDelegate;
	
	/* 특정 퀘스트의 진행도를 올리는 함수 */
	void IncreaseQuestProgress(const FName& QuestIdToUpdate, const int32 IncreaseAmount = 1);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> QuestDataTable;
	
	UPROPERTY(ReplicatedUsing = "OnRep_ActivatedQuests", BlueprintReadOnly, VisibleAnywhere)
	TArray<FQuestInstance> ActivatedQuests;
	
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	TArray<FQuestInstance> CompletedQuests;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FName TrackedQuestId;

private:
	void CreateAllQuestMap();
	void AddActivatedQuest(const FQuestObjectiveBase& QuestObjective);
	
	TMap<FName, FQuestObjectiveBase> AllQuestsMap;
	/* 해당 레벨에 수행할 수 있는 퀘스트 목록 맵 */
	TMap<int32, TArray<FQuestObjectiveBase>> LevelToQuestsMap;
	
	// 퀘스트 완료시 처리하는 함수
	void CompleteQuest(const FName& CompletedQuestId);
	
	// 퀘스트 보상 플레이어게 주는 함수
	void GrantQuestRewardsToPlayer(const FName& QuestId) const;
	
	UFUNCTION(Client, Reliable)
	void Client_SetTrackedQuestId(const FName& CompletedQuestId, const FName& NextQuestId);

	UFUNCTION()
	void OnRep_ActivatedQuests();

public:
	FORCEINLINE TArray<FQuestInstance> GetActivatedQuests() const { return ActivatedQuests; }
	FORCEINLINE TArray<FQuestInstance> GetCompletedQuests() const { return CompletedQuests; }
	FORCEINLINE FName GetTrackedQuestId() const { return TrackedQuestId; }
	
};
