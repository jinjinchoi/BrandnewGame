// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/BrandNewPlayerAnimInstance.h"

#include "Character/BrandNewBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UBrandNewPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	SetMovementComponentData();
	
}


void UBrandNewPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!OwningCharacter) return;
	
	LastFrameGate = CurrentGate;
	CurrentGate = InComingGate;
	bIsGateChanged = CurrentGate != LastFrameGate;
	
	VelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());
	AccelerationLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(Acceleration, OwningCharacter->GetActorRotation());
	
	CalculateJumpState();
	UpdateCharacterLocation();
	CalculatePivotAngle();
	CalculateFallingTime(DeltaSeconds);
}

void UBrandNewPlayerAnimInstance::CalculateJumpState()
{
	bIsJumping = bIsOnAir && Velocity.Z > 0;
	bIsFalling = bIsOnAir && Velocity.Z <= 0;

	if (bIsJumping)
	{
		TimeToJumpApex = (0.f - Velocity.Z) / (OwningMovementComponent->GetGravityZ() * OwningMovementComponent->GravityScale);
	}
	else
	{
		TimeToJumpApex = 0.f;
	}
	
}

void UBrandNewPlayerAnimInstance::CalculateFallingTime(const float DeltaTime)
{
	if (bIsFalling)
	{
		FallingTime += DeltaTime;
	}
	else if (bIsJumping)
	{
		FallingTime = 0.f;
	}

	
}


void UBrandNewPlayerAnimInstance::CalculatePivotAngle()
{
	// 캐릭터가 Pivot 중인지 확인하기 위하여 DotProduct 계산.
	const FVector VelocityNormal = Velocity.GetSafeNormal();
	const FVector AccelerationNormal = Acceleration.GetSafeNormal();
	PivotDirectionDot = FVector::DotProduct(VelocityNormal, AccelerationNormal);
}

void UBrandNewPlayerAnimInstance::MovementInputReceived(const FVector2D& LastInputVector, const FRotator& ControllerRotation)
{
	if (!OwningCharacter) return;
	
	if (RootYawOffsetMode == ERootYawOffsetMode::BlendOut)
	{
		TurnDirection = ETurnDirection::None;
		return;
	}
	
	const FVector InputDir = FVector(LastInputVector.Y, LastInputVector.X, 0.f);
	
	if (InputDir.SizeSquared() > KINDA_SMALL_NUMBER)
	{
		// 카메라 방향
		const FRotator CameraRot = ControllerRotation;
		const FRotator CameraYawRot(0.f, CameraRot.Yaw, 0.f);

		// 입력 벡터를 카메라 기준 월드 방향으로 변환
		const FVector InputDirWS = FRotationMatrix(CameraYawRot).TransformVector(InputDir);
		
		// 입력 방향 → 회전값
		const FRotator InputRot = InputDirWS.ToOrientationRotator();
		
		// 현재 캐릭터가 바라보는 방향
		const FRotator ActorRot = OwningCharacter->GetActorRotation();

		// 두 방향의 차이 (Yaw만)
		const float DeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(InputRot, ActorRot).Yaw;
		const float TurnAngle = FMath::Abs(DeltaYaw);
		
		// 왼쪽/오른쪽 판별
		const bool bTurnRight = (DeltaYaw > 0.f);

		// 회전 타입 판별
		if (TurnAngle > 135.f)
		{
			TurnDirection = bTurnRight ? ETurnDirection::Turn180Right : ETurnDirection::Turn180Left;
		}
		else if (TurnAngle > 45.f)
		{
			TurnDirection = bTurnRight ? ETurnDirection::Turn90Right : ETurnDirection::Turn90Left;
		}
		else
		{
			TurnDirection = ETurnDirection::None;
		}
	}
}

void UBrandNewPlayerAnimInstance::ReceiveGroundDistance(const float InDistance)
{
	GroundDistance = InDistance;
}


void UBrandNewPlayerAnimInstance::UpdateCharacterLocation()
{
	LastFrameCharacterLocation = CharacterLocation;
	CharacterLocation = OwningCharacter->GetActorLocation();
	DeltaLocation = (CharacterLocation - LastFrameCharacterLocation).Length();
}

void UBrandNewPlayerAnimInstance::UpdateCurrentGate(const EGate NewGate)
{
	InComingGate = NewGate;
	SetMovementComponentData();
}



void UBrandNewPlayerAnimInstance::SetMovementComponentData()
{
	if (!OwningMovementComponent) return;
	
	bUseSeparateBrakingFriction = OwningMovementComponent->bUseSeparateBrakingFriction;
	BrakingFriction = OwningMovementComponent->BrakingFriction;
	GroundFriction = OwningMovementComponent->GroundFriction;
	BrakingFrictionFactor = OwningMovementComponent->BrakingFrictionFactor;
	BrakingDecelerationWalking = OwningMovementComponent->BrakingDecelerationWalking;
	MaxWalkSpeed = OwningMovementComponent->MaxWalkSpeed;
}
