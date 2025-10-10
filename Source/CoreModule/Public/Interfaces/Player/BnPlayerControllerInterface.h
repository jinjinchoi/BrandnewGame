// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "UObject/Interface.h"
#include "BnPlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBnPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBnPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddInputMappingForWeapon(const ECombatWeaponType InWeaponType) = 0;
	virtual void RemoveInputMappingForWeapon(const ECombatWeaponType WeaponTypeToRemove) = 0;

	
	
};
