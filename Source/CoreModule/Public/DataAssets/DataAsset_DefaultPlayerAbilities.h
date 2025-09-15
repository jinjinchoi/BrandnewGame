// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "DataAsset_DefaultPlayerAbilities.generated.h"

USTRUCT(BlueprintType)
struct FPlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Input.Ability"))
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityToGrand = nullptr;

	bool IsValid() const
	{
		return InputTag.IsValid() && AbilityToGrand;
	}
	
};

/**
 * 플레이어 캐릭터가 사용할 어빌리티를 저장하는 데이터 에셋.
 * Input이 필요한 어빌리티는 구조체를 통하여 어빌리티 클래스와 InputTag를 매칭시키고 그외에는 어빌리티만 저장.
 */
UCLASS()
class COREMODULE_API UDataAsset_DefaultPlayerAbilities : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerAbilities", meta = (TitleProperty = "InputTag"))
	TArray<FPlayerAbilitySet> InputAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerAbilities", meta = (TitleProperty = "InputTag"))
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerAbilities", meta = (TitleProperty = "InputTag"))
	TArray<TSubclassOf<UGameplayAbility>> ReactAbilities;
};
