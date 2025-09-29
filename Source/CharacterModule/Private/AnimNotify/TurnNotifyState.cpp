// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/TurnNotifyState.h"

#include "Interfaces/Character/BrandNewPlayerInterface.h"

void UTurnNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	LastCurveValue = 0.f;
}

void UTurnNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (IBrandNewPlayerInterface* RotInterface = Cast<IBrandNewPlayerInterface>(MeshComp->GetOwner()))
	{
		const float CurveValue = MeshComp->GetAnimInstance()->GetCurveValue(CurveName);
		const float Delta = CurveValue - LastCurveValue;
		LastCurveValue = CurveValue;

		RotInterface->AddYawRotation(Delta);
	}
	
}
