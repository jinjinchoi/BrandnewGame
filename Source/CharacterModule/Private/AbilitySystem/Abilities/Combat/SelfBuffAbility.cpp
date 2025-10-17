// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/SelfBuffAbility.h"

#include "AbilitySystemComponent.h"

void USelfBuffAbility::ApplyBuffEffectToSelf() const
{
	if (!BuffEffectClass) return;

	if (CurrentActorInfo && BuffEffectClass)
	{
		if (UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get())
		{
			FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(BuffEffectClass, GetAbilityLevel(), ContextHandle);
			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	
}
