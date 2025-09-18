// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/TargetLockAbility.h"

#include "CharacterFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Interfaces/BrandNewCharacterInterface.h"
#include "Kismet/KismetMathLibrary.h"


void UTargetLockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	TryLockOnTarget();
	
}

void UTargetLockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UTargetLockAbility::TryLockOnTarget()
{
	if (!HasAuthority(&CurrentActivationInfo)) return;

	FindClosestActorToMouse();
		
	if (TargetActors.IsEmpty() || !IsValid(ClosestActorToMouse))
	{
		CancelTargetLockAbility();
	}

	if (IsLocallyControlled())
	{
		DrawTargetLockWidget();
		SetTargetLockWidgetPosition();
	}
	
}

void UTargetLockAbility::OnRep_ClosestActorToMouse()
{
	DrawTargetLockWidget();
	SetTargetLockWidgetPosition();
}


void UTargetLockAbility::OnTargetLockTick(float DeltaTime)
{
	if (!IsValid(ClosestActorToMouse) || IBrandNewCharacterInterface::Execute_IsDead(GetAvatarActorFromActorInfo()))
	{
		if (HasAuthority(&CurrentActivationInfo)) CancelTargetLockAbility();
		return;
	}

	if (IBrandNewCharacterInterface::Execute_IsDead(ClosestActorToMouse))
	{
		TargetActors.Remove(ClosestActorToMouse);
		if (TargetActors.IsEmpty())
		{
			CancelTargetLockAbility();
			return;
		}
		if (HasAuthority(&CurrentActivationInfo))
		{
			ClosestActorToMouse = TargetActors.Last();
			Client_SetClosestActor(ClosestActorToMouse);
		}
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation =
		!UCharacterFunctionLibrary::DoseActorHasTag(GetAvatarActorFromActorInfo(), BrandNewGamePlayTag::Status_Player_Dodging);

	if (bShouldOverrideRotation)
	{
		if (!IsValid(ClosestActorToMouse))
		{
			if (HasAuthority(&CurrentActivationInfo)) CancelTargetLockAbility();
			return;
		}
		
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetAvatarActorFromActorInfo()->GetActorLocation(),
			ClosestActorToMouse->GetActorLocation()
		);

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

		const FRotator CurrentControlRot = CurrentActorInfo->PlayerController->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		CurrentActorInfo->PlayerController->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetAvatarActorFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
		
	}
	
}

void UTargetLockAbility::FindClosestActorToMouse()
{
	if (!HasAuthority(&CurrentActivationInfo)) return;
	
	const APlayerController* PlayerController = CurrentActorInfo->PlayerController.Get();

	const FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector TraceDirection = CameraRotation.Vector();
	
	const FVector TraceStart = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector TraceEnd = TraceStart + TraceDirection * 5000.f;

	float MinDistance = FLT_MAX;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	UCharacterFunctionLibrary::GetHostileActorsWithinRadius(
		GetAvatarActorFromActorInfo(),
		GetAvatarActorFromActorInfo(),
		TargetActors,
		ActorsToIgnore,
		5000,
		GetAvatarActorFromActorInfo()->GetActorLocation()
	);

	if (TargetActors.IsEmpty())
	{
		return;
	}

	for (AActor* TargetActor : TargetActors)
	{
		if (!IsValid(TargetActor)) continue;

		const FVector TargetLocation = TargetActor->GetActorLocation();
		FVector ClosestPoint = FMath::ClosestPointOnInfiniteLine(TraceStart, TraceEnd, TargetLocation);

		const float DistToTarget = FVector::DistSquared(TargetActor->GetActorLocation(), ClosestPoint);
		if (DistToTarget < MinDistance)
		{
			MinDistance = DistToTarget;
			ClosestActorToMouse = TargetActor;
		}
	}
	
	Client_SetClosestActor(ClosestActorToMouse);
	
}

void UTargetLockAbility::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UTargetLockAbility::CleanUp()
{
	TargetActors.Empty();
	ClosestActorToMouse = nullptr;
	if (IsLocallyControlled() && IsValid(TargetLockWidget))
	{
		TargetLockWidget->RemoveFromParent();
	}
	TargetLockWidget = nullptr;
}


void UTargetLockAbility::DrawTargetLockWidget()
{
	if (TargetLockWidget) return;
	
	check(TargetLockWidgetClass);

	TargetLockWidget = CreateWidget(CurrentActorInfo->PlayerController.Get(), TargetLockWidgetClass);
	if (TargetLockWidget)
	{
		TargetLockWidget->AddToViewport();
	}
}

void UTargetLockAbility::SetTargetLockWidgetPosition()
{
	if (!IsLocallyControlled()) return;
	
	if (!IsValid(TargetLockWidget) || !IsValid(ClosestActorToMouse))
	{
		if (HasAuthority(&CurrentActivationInfo)) CancelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition = FVector2D::ZeroVector;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		CurrentActorInfo->PlayerController.Get(),
		ClosestActorToMouse->GetActorLocation(),
		ScreenPosition,
		true
	);

	TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
	TargetLockWidget->SetAlignmentInViewport(FVector2D(0.5f, 1.f));
}

void UTargetLockAbility::Client_SetClosestActor_Implementation(AActor* NewTarget)
{
	ClosestActorToMouse = NewTarget;
	DrawTargetLockWidget();
	SetTargetLockWidgetPosition();
}


