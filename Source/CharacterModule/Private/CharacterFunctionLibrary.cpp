// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Kismet/KismetMathLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "Interfaces/Character/BrandNewCharacterInterface.h"

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

FGameplayTag UCharacterFunctionLibrary::GetHitDirectionTagToContext(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const TSharedPtr<FGameplayTag>& HitDirectionTag = GetBrandNewEffectContext(EffectContextHandle).GetHitDirectionTag();
	if (HitDirectionTag.IsValid())
	{
		return *HitDirectionTag;
	}
	return FGameplayTag();
}

bool UCharacterFunctionLibrary::GetIsCriticalHitToContext(const FGameplayEffectContextHandle& EffectContextHandle)
{
	return GetBrandNewEffectContext(EffectContextHandle).IsCriticalHit();
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
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Experience_Level, InAttributePrams.Level);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Experience_AttributePoint, InAttributePrams.AttributePoint);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, BrandNewGamePlayTag::Attribute_Experience_XP, InAttributePrams.XP);

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
	
	return ComputeHitReactDirectionFromLocation(InAttacker->GetActorLocation(), InVictim->GetActorLocation(), InVictim->GetActorForwardVector());
}

FGameplayTag UCharacterFunctionLibrary::ComputeHitReactDirectionFromLocation(const FVector& AttackerLocation, const FVector& VictimLocation, const FVector& VictimForward)
{
	const FVector VictimToAttackerNormalized = (AttackerLocation - VictimLocation).GetSafeNormal();

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

bool UCharacterFunctionLibrary::IsTargetActorHostile(const AActor* QueryActor, const AActor* TargetActor)
{
	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryActor->GetInstigatorController());
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetActor->GetInstigatorController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId(); 
	}

	return false;
}

void UCharacterFunctionLibrary::GetLiveActorWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> OverlapResults;
		
		World->OverlapMultiByObjectType(
			OverlapResults,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);

	
		for (FOverlapResult& Overlap : OverlapResults)
		{
			if (Overlap.GetActor()->IsHidden()) continue;
			
			if (Overlap.GetActor()->Implements<UBrandNewCharacterInterface>() && !IBrandNewCharacterInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
		
	}
}

void UCharacterFunctionLibrary::GetHostileActorsWithinRadius(const UObject* WorldContextObject,
	const AActor* InstigatorActor, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
	float Radius, const FVector& SphereOrigin, bool bDrawDebugSphere)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (bDrawDebugSphere)
		{
			// Origin 위치 메세지로 표시
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Cyan,
				FString::Printf(TEXT("SphereOrigin: %s"), *SphereOrigin.ToString())
			);

			// 범위 구형으로 표시
			DrawDebugSphere(
				World,
				SphereOrigin,
				Radius,
				16, // 세그먼트 (선의 부드러움)
				FColor::Green, // 색상
				false, // 영구 여부 (false면 일정 시간 후 사라짐)
				2.0f, // 지속 시간
				0, // Depth Priority
				1.f // 선 두께
			);

			// Origin 위치 포인트로 표시
			DrawDebugPoint(
				World,
				SphereOrigin,
				10.0f,
				FColor::Yellow,
				false,
				1.0f
			);
		}
		
		TArray<FOverlapResult> OverlapResults;
		
		World->OverlapMultiByObjectType(
			OverlapResults,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);
	
		for (FOverlapResult& Overlap : OverlapResults)
		{
			if (Overlap.GetActor()->IsHidden()) continue;
			if (!Overlap.GetActor()->Implements<UBrandNewCharacterInterface>() || IBrandNewCharacterInterface::Execute_IsDead(Overlap.GetActor())) continue;
			
			if (IsTargetActorHostile(Overlap.GetActor(), InstigatorActor))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

AActor* UCharacterFunctionLibrary::GetClosestActor(const TArray<AActor*>& TargetActors, const FVector& Origin)
{
	if (TargetActors.Num() == 0)
	{
		return nullptr;
	}
	
	AActor* ClosestTarget = nullptr;
	float MinDistSq = TNumericLimits<float>::Max();

	for (AActor* Target : TargetActors)
	{
		if (!IsValid(Target)) continue;

		float DistSq = FVector::DistSquared(Target->GetActorLocation(), Origin);

		if (DistSq < MinDistSq)
		{
			MinDistSq = DistSq;
			ClosestTarget = Target;
		}
	}

	return ClosestTarget;
}

FVector UCharacterFunctionLibrary::GetClosestActorLocation(const TArray<AActor*>& TargetActors, const FVector& Origin)
{
	if (TargetActors.Num() == 0)
	{
		return FVector::ZeroVector;
	}
	
	if (const AActor* ClosestTarget = GetClosestActor(TargetActors, Origin))
	{
		return ClosestTarget->GetActorLocation();
	}

	return FVector::ZeroVector;
}

void UCharacterFunctionLibrary::GetValidGroundLocation(const UObject* WorldContextObject, const FVector& OriginalLocation, FVector& OutValidLocation,
	const float OffsetZ)
{
	if (OriginalLocation.IsNearlyZero())
	{
		OutValidLocation = OriginalLocation;
		return;
	}

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		OutValidLocation = OriginalLocation;
		return;
	}

	const FVector Start = OriginalLocation + FVector(0, 0, 500);
	const FVector End = OriginalLocation - FVector(0, 0, 500);

	FHitResult HitResult;
	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Ground))
	{
		OutValidLocation = HitResult.ImpactPoint + FVector(0, 0, OffsetZ);
	}
}

