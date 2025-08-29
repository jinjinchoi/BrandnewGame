// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"

FBrandNewEffectContext& UCharacterFunctionLibrary::GetBrandNewEffectContext(FGameplayEffectContextHandle& ContextHandle)
{
	FBrandNewEffectContext* BrandNewEffectContext = static_cast<FBrandNewEffectContext*>(ContextHandle.Get());
	check(BrandNewEffectContext);
	
	return *BrandNewEffectContext;
	
}

const FBrandNewEffectContext& UCharacterFunctionLibrary::GetBrandNewEffectContext(const FGameplayEffectContextHandle& ContextHandle)
{
	const FBrandNewEffectContext* BrandNewEffectContext = static_cast<const FBrandNewEffectContext*>(ContextHandle.Get());
	check(BrandNewEffectContext);
	
	return *BrandNewEffectContext;
}

void UCharacterFunctionLibrary::SetDamageTypeTagToContext(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InTypeTag)
{
	if (InTypeTag.IsValid())
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InTypeTag);
		GetBrandNewEffectContext(EffectContextHandle).SetDamageTypeTag(DamageType);
	}
}

void UCharacterFunctionLibrary::SetDamageElementTagToContext(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InElemTag)
{
	if (InElemTag.IsValid())
	{
		const TSharedPtr<FGameplayTag> DamageElem = MakeShared<FGameplayTag>(InElemTag);
		GetBrandNewEffectContext(EffectContextHandle).SetDamageElementTag(DamageElem);
	}
}

FGameplayTag UCharacterFunctionLibrary::GetDamageTypeTagToContext(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const TSharedPtr<FGameplayTag>& DamageSharedTag = GetBrandNewEffectContext(EffectContextHandle).GetDamageTypeTag();
	if (DamageSharedTag.IsValid())
	{
		return *DamageSharedTag;
	}
	return FGameplayTag();
}

FGameplayTag UCharacterFunctionLibrary::GetDamageElementTagToContext( const FGameplayEffectContextHandle& EffectContextHandle)
{
	const TSharedPtr<FGameplayTag>& ElementSharedTag = GetBrandNewEffectContext(EffectContextHandle).GetDamageElementTag();
	if (ElementSharedTag.IsValid())
	{
		return *ElementSharedTag;
	}
	return FGameplayTag();
}


void UCharacterFunctionLibrary::ApplyPrimaryAttributesSetByCaller(const FBaseAttributePrams& InAttributePrams,
                                                                  UAbilitySystemComponent* InASC, const TSubclassOf<UGameplayEffect>& EffectToApply)
{
	if (!InASC || !EffectToApply) return;

	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	ContextHandle.AddSourceObject(InASC->GetAvatarActor());
	
	const FGameplayEffectSpecHandle EffectSpecHandle = InASC->MakeOutgoingSpec(EffectToApply, 1.f, ContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Primary_Strength, InAttributePrams.Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Primary_Dexterity, InAttributePrams.Dexterity);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Primary_Intelligence, InAttributePrams.Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Primary_Vitality, InAttributePrams.Vitality);

	InASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

bool UCharacterFunctionLibrary::DoseActorHasTag(AActor* TargetActor, const FGameplayTag& TagToCheck)
{
	if (!IsValid(TargetActor)) return false;

	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		return ASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

void UCharacterFunctionLibrary::AddGameplayTagToActor(AActor* TargetActor, const FGameplayTag& TagToAdd)
{
	if (!IsValid(TargetActor)) return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		if (!ASC->HasMatchingGameplayTag(TagToAdd))
		{
			ASC->AddLooseGameplayTag(TagToAdd);
		}
	}
	
}

void UCharacterFunctionLibrary::RemoveGameplayTagToActor(AActor* TargetActor, const FGameplayTag& TagToAdd)
{
	if (!IsValid(TargetActor)) return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		if (ASC->HasMatchingGameplayTag(TagToAdd))
		{
			ASC->RemoveLooseGameplayTag(TagToAdd);
		}
	}
}

FActiveGameplayEffectHandle UCharacterFunctionLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	if (!DamageEffectParams.IsValid()) return FActiveGameplayEffectHandle();
	
	FGameplayEffectContextHandle ContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor());

	SetDamageElementTagToContext(ContextHandle, DamageEffectParams.DamageElement);
	SetDamageTypeTagToContext(ContextHandle, DamageEffectParams.DamageType);
	
	const FGameplayEffectSpecHandle SpecHandle =
		DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
			DamageEffectParams.DamageGameplayEffectClass,
			DamageEffectParams.AbilityLevel,
			ContextHandle
		);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.DamageCoefficient);

	return DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

