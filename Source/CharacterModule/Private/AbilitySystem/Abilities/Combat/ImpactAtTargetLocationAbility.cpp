// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/ImpactAtTargetLocationAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CharacterFunctionLibrary.h"
#include "BrandNewTypes/BrandNewMacro.h"


void UImpactAtTargetLocationAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	TargetLocations.Empty();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UImpactAtTargetLocationAbility::SpawnWarningEffect()
{
	if (!WarningEffectTag.IsValid() || !HasAuthority(&CurrentActivationInfo)) return;
	
	UpdateTargetLocationArray();

	for (const FVector& SpawnLocation : TargetLocations)
	{
		FGameplayCueParameters WaningEffectCueParams;
		WaningEffectCueParams.Location = SpawnLocation;
		WaningEffectCueParams.Instigator = GetAvatarActorFromActorInfo();
		WaningEffectCueParams.RawMagnitude = WarningEffectMagnitude;

		GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(WarningEffectTag, WaningEffectCueParams);
	}

	CachedTargetLocationQueue.Enqueue(TargetLocations);
}

void UImpactAtTargetLocationAbility::AttackAtTargetLocation()
{
	if (!AttackEffectTag.IsValid() || CachedTargetLocationQueue.IsEmpty() || !HasAuthority(&CurrentActivationInfo)) return;

	TArray<FVector> CachedTargetLocation;
	CachedTargetLocationQueue.Dequeue(CachedTargetLocation);
	
	for (const FVector& SpawnLocation : CachedTargetLocation)
	{
		FGameplayCueParameters WaningEffectCueParams;
		WaningEffectCueParams.Location = SpawnLocation;
		WaningEffectCueParams.Instigator = GetAvatarActorFromActorInfo();
		WaningEffectCueParams.RawMagnitude = AttackEffectMagnitude;
		
		GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(AttackEffectTag, WaningEffectCueParams);

		TArray<AActor*> HostileActors;
		const FVector AttackOrigin = SpawnLocation + FVector(0.f, 0.f, DamageOriginOffsetZ);
		
		UCharacterFunctionLibrary::GetHostileActorsWithinRadius(
			GetAvatarActorFromActorInfo(),
			GetAvatarActorFromActorInfo(),
			HostileActors,
			TArray<AActor*>(),
			DamageRadius,
			AttackOrigin
		);

		if (HostileActors.IsEmpty()) continue;

		for (AActor* TargetActor : HostileActors)
		{
			if (!TargetActor) continue;
			
			const FDamageEffectParams DamageEffectParams = MakeLocationDamageAbilityDamageParams(TargetActor, AttackOrigin);
			UCharacterFunctionLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}



void UImpactAtTargetLocationAbility::UpdateTargetLocationArray()
{
	TArray<AActor*> HostileActors;
	
	UCharacterFunctionLibrary::GetHostileActorsWithinRadius(
		GetAvatarActorFromActorInfo(),
		GetAvatarActorFromActorInfo(),
		HostileActors,
		TArray<AActor*>(),
		AbilityRange,
		GetAvatarActorFromActorInfo()->GetActorLocation()
	);
	
	if (HostileActors.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
	
	TArray<FVector> HostileActorLocationArray;
	for (const AActor* TargetActor : HostileActors)
	{
		if (TargetActor)
		{
			const FVector OriginalLocation = TargetActor->GetActorLocation();
			FVector ValidLocation = FVector::ZeroVector;
			GetValidLocation(OriginalLocation, ValidLocation);
			
			HostileActorLocationArray.AddUnique(ValidLocation);
		}
	}

	TargetLocations = HostileActorLocationArray;
	
}

void UImpactAtTargetLocationAbility::GetValidLocation(const FVector& OriginalLocation, FVector& OutValidLocation, const float OffsetZ) const
{
	if (OriginalLocation.IsNearlyZero())
	{
		OutValidLocation = OriginalLocation;
		return;
	}

	const FVector Start = OriginalLocation + FVector(0, 0, 500);
	const FVector End = OriginalLocation - FVector(0, 0, 500);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Ground))
	{
		OutValidLocation = HitResult.ImpactPoint + FVector(0, 0, OffsetZ);
	}
	
}


