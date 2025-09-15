// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BrandNewCharacterInterface.generated.h"

class UMotionWarpingComponent;

DECLARE_MULTICAST_DELEGATE(FOnCharacterDied);


// This class does not need to be modified.
UINTERFACE()
class UBrandNewCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBrandNewCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleWeaponCollision(bool bEnable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCharacterHit(const bool bIsHit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnCharacterDied();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UMotionWarpingComponent* GetMotionWarpingComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetProjectileSpawnLocation(const FName& SocketName) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	virtual bool IsHitReacting() const = 0;
	virtual FOnCharacterDied& GetOnCharacterDiedDelegate() = 0;
	

	
};
