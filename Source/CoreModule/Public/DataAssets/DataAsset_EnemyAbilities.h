// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_EnemyAbilities.generated.h"

class UGameplayAbility;
/**
 * 에너미의 어빌리티를 설정하는 데이터 에셋
 */
UCLASS()
class COREMODULE_API UDataAsset_EnemyAbilities : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> ReactAbilities;
	
};
