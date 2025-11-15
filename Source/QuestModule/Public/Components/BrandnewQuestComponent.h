// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestType/QuestType.h"
#include "BrandnewQuestComponent.generated.h"

class UBnQuestInstance;

USTRUCT(BlueprintType)
struct FQuestInstance
{
	GENERATED_BODY()
	
	FName QuestId = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	FQuestObjectiveBase QuestObjective = FQuestObjectiveBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	EQuestState QuestState = EQuestState::InProgress;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Brandnew|Quest")
	int32 CurrentCount = 0;
	
	bool operator==(const FQuestInstance& Other) const
	{
		return QuestId == Other.QuestId;
	}
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTMODULE_API UBrandnewQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBrandnewQuestComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* 레벨을 보내면 해당 레벨에 수행 가능한 퀘스트를 추가해주는 함수 */
	void GrantQuestByLevelRequirement(const int32 PlayerLevel);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> QuestDataTable;
	
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	TArray<FQuestInstance> ActivatedQuests;
	
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	TArray<FQuestInstance> CompletedQuests;

private:
	void CreateAllQuestMap();
	
	TMap<FName, FQuestObjectiveBase> AllQuestsMap;
	/* 해당 레벨에 수행할 수 있는 퀘스트 목록 맵 */
	TMap<int32, TArray<FQuestObjectiveBase>> LevelToQuestsMap;
	
	
};
