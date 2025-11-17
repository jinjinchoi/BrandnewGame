// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BrandnewQuestSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandnewQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void AddQuestActorToMap(const FName& ActorId, AActor* Actor);
	void RemoveQuestActorFromMap(const FName& ActorId);
	
	AActor* GetActorFromMap(const FName& ActorId);
	
protected:
	UPROPERTY()
	TMap<FName, AActor*> QuestActorMap;
	
};
