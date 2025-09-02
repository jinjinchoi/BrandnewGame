// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"

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

void UCharacterFunctionLibrary::SetHitDirectionTag(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InHitDirectionTag)
{
	if (InHitDirectionTag.IsValid())
	{
		const TSharedPtr<FGameplayTag> HitDirectionTag = MakeShared<FGameplayTag>(InHitDirectionTag);
		GetBrandNewEffectContext(EffectContextHandle).SetHitDirectionTag(HitDirectionTag);
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

FGameplayTag UCharacterFunctionLibrary::GetHitDirectionTag(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const TSharedPtr<FGameplayTag>& HitDirectionTag = GetBrandNewEffectContext(EffectContextHandle).GetHitDirectionTag();
	if (HitDirectionTag.IsValid())
	{
		return *HitDirectionTag;
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
	SetHitDirectionTag(ContextHandle, DamageEffectParams.HitDirection);

	GetBrandNewEffectContext(ContextHandle).SetKnockbackImpulse(DamageEffectParams.KnockbackImpulse);
	
	const FGameplayEffectSpecHandle SpecHandle =
		DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(
			DamageEffectParams.DamageGameplayEffectClass,
			DamageEffectParams.AbilityLevel,
			ContextHandle
		);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.DamageCoefficient);

	return DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

FGameplayTag UCharacterFunctionLibrary::ComputeHitReactDirection(const AActor* InAttacker, const AActor* InVictim)
{
	if (!IsValid(InAttacker) || !IsValid(InVictim))
	{
		return BrandNewGamePlayTag::Event_HitReact_Front;
	}

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	float AngleDifference = UKismetMathLibrary::DegAcos(DotResult);

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

	if (CrossResult.Z < 0.f)
	{
		AngleDifference *= -1.f;
	}

	if (AngleDifference >= -45.f && AngleDifference <= 45.f)
	{
		return BrandNewGamePlayTag::Event_HitReact_Front;
	}
	else if (AngleDifference < -45.f && AngleDifference >= -135.f)
	{
		return BrandNewGamePlayTag::Event_HitReact_Left;
	}
	else if (AngleDifference < -135.f || AngleDifference > 135.f)
	{
		return BrandNewGamePlayTag::Event_HitReact_Back;
	}
	else if (AngleDifference > 45.f && AngleDifference <= 135.f)
	{
		return BrandNewGamePlayTag::Event_HitReact_Right;
	}

	return BrandNewGamePlayTag::Event_HitReact_Front;
}

