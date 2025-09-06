// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ActiveAbilityByTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"


UBTTask_ActiveAbilityByTag::UBTTask_ActiveAbilityByTag()
{
	NodeName = TEXT("Native Execute Ability By Tag");
	bCreateNodeInstance = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();
	
}




FString UBTTask_ActiveAbilityByTag::GetStaticDescription() const
{
	const FString TagName = AbilityTag.GetTagName().ToString();
	return FString::Printf(TEXT("Receive %s and Try Activate Ability"), *TagName);
}


uint16 UBTTask_ActiveAbilityByTag::GetInstanceMemorySize() const
{
	return sizeof(FActiveAbilityByTagTaskMemory);
}


EBTNodeResult::Type UBTTask_ActiveAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!AbilityTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBrandNewAbilitySystemComponent* OwningASC = Cast<UBrandNewAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn));

	FActiveAbilityByTagTaskMemory* Memory = CastInstanceNodeMemory<FActiveAbilityByTagTaskMemory>(NodeMemory);
	check(Memory);

	Memory->OwningPawn = OwningPawn;
	Memory->AbilitySystemComponent = OwningASC;
	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	FGameplayAbilitySpec* Spec = OwningASC->FindAbilitySpecFromAbilityTag(AbilityTag);
	if (!Spec)
	{
		return EBTNodeResult::Failed;
	}
	
	FGameplayAbilitySpecHandle SpecHandle = Spec->Handle;

	if (OwningASC->TryActivateAbility(SpecHandle))
	{
		TWeakObjectPtr WeakThis(this);
		TWeakObjectPtr WeakComp(&OwnerComp);
		
		Memory->ActivatedAbilitySpecHandle = SpecHandle;
		Memory->OnAbilityEndedDelegateHandle = OwningASC->OnAbilityEnded.AddLambda(
			[WeakThis, WeakComp, NodeMemory](const FAbilityEndedData& Data)
			{
				if (!WeakThis.IsValid() || !WeakComp.IsValid() || !NodeMemory)
				{
					return;
				}

				FActiveAbilityByTagTaskMemory* LambdaMemory = WeakThis->CastInstanceNodeMemory<FActiveAbilityByTagTaskMemory>(NodeMemory);
				if (LambdaMemory && LambdaMemory->IsValid())
				{
					if (Data.AbilitySpecHandle == LambdaMemory->ActivatedAbilitySpecHandle)
					{
						WeakThis->FinishLatentTask(*WeakComp, Data.bWasCancelled ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
					}
					
				}
			});
		
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}


void UBTTask_ActiveAbilityByTag::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	if (FActiveAbilityByTagTaskMemory* Memory = CastInstanceNodeMemory<FActiveAbilityByTagTaskMemory>(NodeMemory))
	{
		Memory->AbilitySystemComponent->OnAbilityEnded.Remove(Memory->OnAbilityEndedDelegateHandle);
		Memory->Reset();
	}
	
}

EBTNodeResult::Type UBTTask_ActiveAbilityByTag::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (FActiveAbilityByTagTaskMemory* Memory = CastInstanceNodeMemory<FActiveAbilityByTagTaskMemory>(NodeMemory))
	{
		Memory->AbilitySystemComponent->CancelAbilityHandle(Memory->ActivatedAbilitySpecHandle);
		Memory->AbilitySystemComponent->OnAbilityEnded.Remove(Memory->OnAbilityEndedDelegateHandle);
		Memory->Reset();
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}





