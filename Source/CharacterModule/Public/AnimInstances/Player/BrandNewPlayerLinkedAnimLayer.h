// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BrandNewBaseAnimInstance.h"
#include "BrandNewPlayerLinkedAnimLayer.generated.h"

class UBrandNewPlayerAnimInstance;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewPlayerLinkedAnimLayer : public UBrandNewBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UBrandNewPlayerAnimInstance* GetPlayerAnimInstance() const;

	
};
