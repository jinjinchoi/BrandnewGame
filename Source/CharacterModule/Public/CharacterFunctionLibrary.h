// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BrandNewAbilityTypes.h"
#include "AbilitySystem/Abilities/Combat/BandNewDamageAbilityBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CharacterFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UCharacterFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* custom context 가져오는 함수로 읽기/쓰기 가능 */
	static FBrandNewEffectContext& GetBrandNewEffectContext(FGameplayEffectContextHandle& ContextHandle);
	
	/* custom context 가져오는 함수로 읽기만 가능 */
	static const FBrandNewEffectContext& GetBrandNewEffectContext(const FGameplayEffectContextHandle& ContextHandle);

	static void SetDamageTypeTagToContext(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InTypeTag);
	static void SetDamageElementTagToContext(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InElemTag);
	static void SetHitDirectionTag(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InHitDirectionTag);
	
	static FGameplayTag GetDamageTypeTagToContext(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static FGameplayTag GetDamageElementTagToContext(const FGameplayEffectContextHandle& EffectContextHandle);
	static FGameplayTag GetHitDirectionTagToContext(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure)
	static bool GetIsCriticalHitToContext(const FGameplayEffectContextHandle& EffectContextHandle);
	
	static void ApplyPrimaryAttributesSetByCaller(
		const FBaseAttributePrams& InAttributePrams, UAbilitySystemComponent* InASC, const TSubclassOf<UGameplayEffect>& EffectToApply);

	UFUNCTION(BlueprintPure, Category = "UCharacterFunctionLibrary|Gameplay Tag")
	static bool DoseActorHasTag(AActor* TargetActor, const FGameplayTag& TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "UCharacterFunctionLibrary|Gameplay Tag")
	static void AddGameplayTagToActor(AActor* TargetActor, const FGameplayTag& TagToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "UCharacterFunctionLibrary|Gameplay Tag")
	static void RemoveGameplayTagToActor(AActor* TargetActor, const FGameplayTag& TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "UCharacterFunctionLibrary|Gameplay Ability")
	static FActiveGameplayEffectHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	static FGameplayTag ComputeHitReactDirection(const AActor* InAttacker, const AActor* InVictim);

	static bool IsTargetActorHostile(const AActor* QueryActor, const AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="UCharacterFunctionLibrary|Gameplay Mechanics")
	static void GetLiveActorWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category="UCharacterFunctionLibrary|Gameplay Mechanics")
	static void GetHostileActorsWithinRadius(const UObject* WorldContextObject, const AActor* InstigatorActor, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
	float Radius, const FVector& SphereOrigin, bool bDrawDebugSphere = false);
	
	static AActor* GetClosestActor(const TArray<AActor*>& TargetActors, const FVector& Origin);
	static FVector GetClosestActorLocation(const TArray<AActor*>& TargetActors, const FVector& Origin);
	
};
