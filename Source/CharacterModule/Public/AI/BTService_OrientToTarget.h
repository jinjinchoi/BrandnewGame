// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_OrientToTarget.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBTService_OrientToTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_OrientToTarget();
	virtual FString GetStaticDescription() const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Target")
	float RotationInterpSpeed; 
	
};
