// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "Components/BrandnewQuestComponent.h"
#include "QuestWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class UIMODULE_API UQuestWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	TArray<FQuestInstance> GetActivatedQuests() const;
	
	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	FQuestInstance GetQuestInstanceById(const FName& QuestId) const;

	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	FQuestObjectiveBase FindQuestObjectiveById(const FName& QuestId) const;
	
};
