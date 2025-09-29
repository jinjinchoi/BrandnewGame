// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BnBaseAnimInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBnBaseAnimInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBnBaseAnimInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetIsStrafing(const bool InIsStrafing) = 0;
	
};
