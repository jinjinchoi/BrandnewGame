// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Actor/BrandNewNPCInterface.h"
#include "BrandNewNPC.generated.h"

class UWidgetComponent;
class USphereComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewNPC : public AActor, public IBrandNewNPCInterface
{
	GENERATED_BODY()

public:
	ABrandNewNPC();
	virtual FName GetFirstDialogueId() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Dialogue")
	FName FirstDialogueId;

private:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
