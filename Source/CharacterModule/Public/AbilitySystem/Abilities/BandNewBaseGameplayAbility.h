// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BandNewBaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBandNewBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	float GetManaCost(const float AbilityLevel) const;
	float GetManaCost() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Brandnew|Cooldown", meta=(Categories = "Cooldown"))
	FGameplayTagContainer CooldownTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Brandnew|Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
	
	
};
