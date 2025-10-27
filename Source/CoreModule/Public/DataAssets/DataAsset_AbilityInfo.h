// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInfoParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability.Player"))
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Cooldown"))
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> AbilityIcon = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	float RequiredMana = 0.f;

	
};


/**
 * 어빌리티 정보를 UI에 보여주기 위해 사용하는 데이터 에셋입니다.
 */
UCLASS()
class COREMODULE_API UDataAsset_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Information", meta = (TitleProperty = "AbilityTag"))
	TArray<FAbilityInfoParams> AbilityInformation;
	
	FAbilityInfoParams FindAbilityInfoByAbilityTag(const FGameplayTag& AbilityTagToFind);
	
	
};
