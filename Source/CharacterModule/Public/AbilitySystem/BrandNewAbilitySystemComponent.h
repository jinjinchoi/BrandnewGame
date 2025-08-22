// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BrandNewAbilitySystemComponent.generated.h"

struct FPlayerAbilitySet;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 어빌리티를 액티브(주로 에너미용)와 패시브로 구분하여 GAS에 부여하는 함수
	void GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant, const bool bIsPassiveAbility = false);
	// InputTag가 존재하는 어빌리티를 부려하는 함수(플레이어 전용)
	void GrantPlayerInputAbilities(const TArray<FPlayerAbilitySet>& AbilitySet);
	// InputTag를 받아와 어빌리티를 실행
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	
};
