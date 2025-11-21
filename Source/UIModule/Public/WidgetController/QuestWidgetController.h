// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewWidgetControllerBase.h"
#include "Components/BrandnewQuestComponent.h"
#include "QuestWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackedQuestChangedDelegate, const FName&, NewTrackedQuestId);

/**
 * 
 */
UCLASS()
class UIMODULE_API UQuestWidgetController : public UBrandNewWidgetControllerBase
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() const override;
	
	
	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	TArray<FQuestInstance> GetActivatedQuests() const;
	
	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	FQuestInstance GetQuestInstanceById(const FName& QuestId) const;

	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	FQuestObjectiveBase FindQuestObjectiveById(const FName& QuestId) const;
	
	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	FQuestInstance FindTrackedQuestInstance() const;
	
	UFUNCTION(BlueprintPure, Category = "Brandnew|QuestSystem")
	FName GetTrackedQuestId() const;
	
	UFUNCTION(BlueprintCallable, Category = "Brandnew|QuestSystem")
	void SetTrackedQuestId(const FName& QuestId);
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FOnTrackedQuestChangedDelegate OnTrackedQuestChangedDelegate;
	
private:
	UBrandnewQuestComponent* GetQuestComponent() const;
	
};
