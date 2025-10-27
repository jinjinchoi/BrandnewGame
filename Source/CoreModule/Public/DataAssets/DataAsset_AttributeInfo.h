// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FBrandNewAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Attribute"))
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayAttribute AttributeGetter;
	
};

/**
 * Attribute의 정보를 UI에 보여줄 때 사용하는 데이터 에셋
 */
UCLASS()
class COREMODULE_API UDataAsset_AttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FGameplayAttribute FindAttributeGetter(const FGameplayTag& InAttributeTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "{AttributeName}"))
	TArray<FBrandNewAttributeInfo> AttributeInformation;
	
};
