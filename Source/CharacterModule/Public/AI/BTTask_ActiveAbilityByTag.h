// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActiveAbilityByTag.generated.h"

class UBrandNewAbilitySystemComponent;
struct FAbilityEndedData;

struct FActiveAbilityByTagTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn = nullptr;
	TWeakObjectPtr<UBrandNewAbilitySystemComponent> AbilitySystemComponent = nullptr;
	FDelegateHandle OnAbilityEndedDelegateHandle;
	FGameplayAbilitySpecHandle ActivatedAbilitySpecHandle;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && AbilitySystemComponent.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		AbilitySystemComponent.Reset();
		OnAbilityEndedDelegateHandle.Reset();
		ActivatedAbilitySpecHandle = FGameplayAbilitySpecHandle();
	}
	
};

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBTTask_ActiveAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_ActiveAbilityByTag();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	
	UPROPERTY(EditAnywhere, Category = "Gameplay Abtility System")
	FGameplayTag AbilityTag;
	
	
};
