// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_OrientToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrientToTarget::UBTService_OrientToTarget()
{
	NodeName = TEXT("Native Orient Rotation to Target Actor");
	
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	
	RotationInterpSpeed = 5.f;
	Interval = 0.f;
	RandomDeviation = 0.f;
	
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, BlackboardKey), AActor::StaticClass());
}

FString UBTService_OrientToTarget::GetStaticDescription() const
{
	const FString KeyDescription = BlackboardKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
	const AActor* TargetActor = Cast<AActor>(ActorObject);
	
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
	
}
