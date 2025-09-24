// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BrandNewAbilitySystemComponent.h"

#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "DataAssets/DataAsset_DefaultPlayerAbilities.h"

void UBrandNewAbilitySystemComponent::GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant,
                                                     const bool bIsPassiveAbility)
{
	if (AbilitiesToGrant.Num() == 0) return;

	for (const TSubclassOf<UGameplayAbility>& AbilityToGrant : AbilitiesToGrant)
	{
		if (!AbilityToGrant) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();

		bIsPassiveAbility ? GiveAbilityAndActivateOnce(AbilitySpec) : GiveAbility(AbilitySpec);
		
	}
	
}

void UBrandNewAbilitySystemComponent::GrantPlayerInputAbilities(const TArray<FPlayerAbilitySet>& AbilitySet)
{
	if (AbilitySet.Num() == 0) return;

	for (const FPlayerAbilitySet& AbilityParams : AbilitySet)
	{
		if (!AbilityParams.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityParams.AbilityToGrand);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityParams.InputTag);

		GiveAbility(AbilitySpec);
	}
	
}

void UBrandNewAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		AbilitySpecInputPressed(AbilitySpec);
		if (AbilitySpec.IsActive())
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.GetAbilityInstances().Last()->GetCurrentActivationInfo().GetActivationPredictionKey());
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UBrandNewAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		if (AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);

			if (const UGameplayAbility* AbilityInstance = AbilitySpec.GetPrimaryInstance())
			{
				const FGameplayAbilityActivationInfo& Info = AbilityInstance->GetCurrentActivationInfo();

				InvokeReplicatedEvent(
					EAbilityGenericReplicatedEvent::InputReleased,
					AbilitySpec.Handle,
					Info.GetActivationPredictionKey());
			}
		}
	}
}

FGameplayAbilitySpecHandle UBrandNewAbilitySystemComponent::FindAbilitySpecHandleFromAbilityTag(const FGameplayTag& AbilityTag)
{
	ABILITYLIST_SCOPE_LOCK();
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (const UGameplayAbility* Ability = AbilitySpec.Ability.Get())
		{
			if (Ability->GetAssetTags().HasTagExact(AbilityTag))
			{
				return AbilitySpec.Handle;
			}
		}
	}

	return FGameplayAbilitySpecHandle();
}

TMap<FGameplayTag, int32> UBrandNewAbilitySystemComponent::GetAbilityTagLevelMap()
{
	ABILITYLIST_SCOPE_LOCK();

	TMap<FGameplayTag, int32> AbilityLevelMap;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (const UGameplayAbility* Ability = AbilitySpec.Ability.Get())
		{
			if (Ability->GetAssetTags().HasTag(BrandNewGamePlayTag::Ability_Player_Combat))
			{
				AbilityLevelMap.Emplace(Ability->GetAssetTags().First(), Ability->GetAbilityLevel());
			}
		}
	}

	return AbilityLevelMap;
}
