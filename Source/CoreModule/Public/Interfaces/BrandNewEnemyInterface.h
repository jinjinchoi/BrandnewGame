// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BrandNewEnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBrandNewEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBrandNewEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetXPReward() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetInitialMaxWalkSpeed() const;
	
};
