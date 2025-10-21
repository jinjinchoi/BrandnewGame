// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Actor/BrandNewNPCInterface.h"
#include "BrandNewNPC.generated.h"

class UCapsuleComponent;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewNPC : public AActor, public IBrandNewNPCInterface
{
	GENERATED_BODY()

public:
	ABrandNewNPC();
	virtual FName GetFirstDialogueId() const override;
	virtual void HideInteractionWidget() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USphereComponent> SphereCollision;

	// NPC 네임은 블루프린트에서 설정중
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> NPCNameWidgetComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Dialogue")
	FName FirstDialogueId;

private:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
