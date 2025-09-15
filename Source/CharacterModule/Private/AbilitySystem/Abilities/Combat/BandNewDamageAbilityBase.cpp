// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/BandNewDamageAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CharacterFunctionLibrary.h"

FDamageEffectParams UBandNewDamageAbilityBase::MakeDamageEffectParams(AActor* TargetActor) const
{
	if (!GetAvatarActorFromActorInfo() || !IsValid(TargetActor)) return FDamageEffectParams();
	
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageCoefficient = DamageCoefficient.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DamageElement = DamageElement;
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	DamageEffectParams.HitDirection = UCharacterFunctionLibrary::ComputeHitReactDirection(GetAvatarActorFromActorInfo(), TargetActor);

	if (KnockbackForce != 0.f)
	{
		const FVector ToTarget = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
		DamageEffectParams.KnockbackImpulse = ToTarget * KnockbackForce;
	}
	

	return DamageEffectParams;
	
}

FDamageEffectParams UBandNewDamageAbilityBase::MakeNonTargetEffectParams() const
{
	check(DamageEffectClass);
	if (!GetAvatarActorFromActorInfo()) return FDamageEffectParams();
	
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageCoefficient = DamageCoefficient.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DamageElement = DamageElement;

	return DamageEffectParams;
	
}
