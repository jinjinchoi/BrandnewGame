// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Quest/BrandnewQuestActor.h"
#include "QuestTriggerActor.generated.h"

class UBoxComponent;

UCLASS()
class CHARACTERMODULE_API AQuestTriggerActor : public ABrandnewQuestActor
{
	GENERATED_BODY()

public:
	AQuestTriggerActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
