// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BrandNewCharacterAnimInstance.h"
#include "Interfaces/Animation/BrandNewPlayerAnimInterface.h"
#include "BrandNewPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewPlayerAnimInstance : public UBrandNewCharacterAnimInstance, public IBrandNewPlayerAnimInterface
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	/* begin IBrandNewPlayerAnimInterface */
	virtual void UpdateCurrentGate(const EGate NewGate) override;
	virtual void MovementInputReceived(const FVector2D& LastInputVector, const FRotator& ControllerRotation) override;
	virtual void ReceiveGroundDistance(const float InDistance) override;
	/* end IBrandNewPlayerAnimInterface */

protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|WeaponData")
	ECombatWeaponType CurrentWeaponType = ECombatWeaponType::Unequipped;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|Gate")
	EGate InComingGate = EGate::Jogging;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|Gate")
	EGate CurrentGate = EGate::Jogging;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|Gate")
	EGate LastFrameGate = EGate::Jogging;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|Gate")
	bool bIsGateChanged = false;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|LocationData")
	FVector CharacterLocation = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|LocationData")
	FVector LastFrameCharacterLocation = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|LocationData")
	float DeltaLocation = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|VelocityData")
	float MaxWalkSpeed = 0.f;

	// 캐릭터가 이동중 방향이 바뀌었을때 어느정도 바뀌었는지 저장하는 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|PivotData")
	float PivotDirectionDot = 0.f;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Native|PivotData")
	FVector PivotAcceleration2D = FVector::ZeroVector;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|LocomotionData")
	float AccelerationLocomotionAngle = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|JumpData")
	bool bIsJumping = false;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|JumpData")
	bool bIsFalling = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|JumpData")
	float TimeToJumpApex = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Native|JumpData")
	float GroundDistance = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Native|JumpData")
	float FallingTime = 0.f;

#pragma region TurnInPlace

	// 캐릭터의 회전 방향
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Native|TurnInPlace")
	ETurnDirection TurnDirection;
	
	// Turn In Place 실행 여부 나타내는 Enum
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Native|TurnInPlace")
	ERootYawOffsetMode RootYawOffsetMode = ERootYawOffsetMode::Setting;

#pragma endregion 

///////////////////////////
// Predict 함수를 위한 변수들
///////////////////////////
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ForLinkedAnim|LocomotionData")
	bool bUseSeparateBrakingFriction = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ForLinkedAnim|LocomotionData")
	float BrakingFriction = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ForLinkedAnim|LocomotionData")
	float GroundFriction = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ForLinkedAnim|LocomotionData")
	float BrakingFrictionFactor = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="ForLinkedAnim|LocomotionData")
	float BrakingDecelerationWalking = 0.f;

private:
	void SetMovementComponentData();
	void UpdateCharacterLocation();
	void CalculatePivotAngle();
	void CalculateJumpState();
	void CalculateFallingTime(const float DeltaTime);

};
