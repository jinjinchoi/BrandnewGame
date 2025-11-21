// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BrandnewQuestSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnQuestActorSet)

/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandnewQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void AddQuestActorToMap(const FName& ActorId, AActor* Actor);
	
	UFUNCTION(BlueprintCallable, Category = "Brandnew|Subsystem Function")
	void ClearSubsystem();
	
	AActor* GetQuestTargetById(const FName& ActorId);
	
	FOnQuestActorSet OnQuestActorSetDelegate;

	
protected:
	UPROPERTY()
	TMap<FName, AActor*> QuestActorMap;
	
private:
	// 퀘스트 액터 요청 했지만 아직 월드에 존재하지 않을 때 등록된 후 사용할 id
	FName PendingActorId;
	
};
