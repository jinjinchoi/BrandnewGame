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
 * 
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
