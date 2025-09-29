// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BrandNewBaseAnimInstance.h"
#include "Interfaces/Animation/BnBaseAnimInstanceInterface.h"
#include "BrandNewCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewCharacterAnimInstance : public UBrandNewBaseAnimInstance, public IBnBaseAnimInstanceInterface 
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void SetIsStrafing(const bool InIsStrafing) override;

protected:
	UPROPERTY()
	TObjectPtr<ABrandNewBaseCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|VelocityData")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|VelocityData")
	float VelocityLength2D = 0.f;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|VelocityData")
	float GroundSpeed = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|VelocityData")
	bool bIsMoving = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|AccelerationData")
	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|AccelerationData")
	bool bHasAcceleration = false;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|JumpData")
	bool bIsOnAir = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|LocomotionData")
	float VelocityLocomotionAngle = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|CombatData")
	bool bIsStrafing = false;
	
};
