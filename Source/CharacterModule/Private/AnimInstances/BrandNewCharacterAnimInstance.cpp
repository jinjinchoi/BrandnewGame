// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BrandNewCharacterAnimInstance.h"

#include "CharacterFunctionLibrary.h"
#include "KismetAnimationLibrary.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Character/BrandNewBaseCharacter.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBrandNewCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ABrandNewBaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
	
}

void UBrandNewCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwningCharacter) || !OwningMovementComponent) return;

	Velocity = OwningCharacter->GetVelocity();
	VelocityLength2D = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	GroundSpeed = Velocity.Size2D();
	bIsMoving = Velocity.SizeSquared2D() > KINDA_SMALL_NUMBER;

	Acceleration = OwningMovementComponent->GetCurrentAcceleration();
	bHasAcceleration = Acceleration.SizeSquared2D() > 0.f;
	
	bIsOnAir = OwningMovementComponent->IsFalling();

	VelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());
	
}

void UBrandNewCharacterAnimInstance::SetIsStrafing(const bool InIsStrafing)
{
	bIsStrafing = InIsStrafing;
}
