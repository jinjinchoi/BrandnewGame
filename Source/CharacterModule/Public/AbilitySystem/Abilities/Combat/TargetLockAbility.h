// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BandNewBaseGameplayAbility.h"
#include "TargetLockAbility.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UTargetLockAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	/* Begin UGameplayAbility Interface */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	/* End UGameplayAbility Interface */

	
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);
	
	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Target Lock")
	TSubclassOf<UUserWidget> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Target Lock")
	float TargetLockCameraOffsetDistance = 10.f;	

private:
	void TryLockOnTarget();
	void FindClosestActorToMouse();
	void DrawTargetLockWidget();
	void CancelTargetLockAbility();
	void CleanUp();
	void SetTargetLockWidgetPosition();

	UFUNCTION(Client, Reliable)
	void Client_SetClosestActor(AActor* NewTarget);
	
	UPROPERTY()
	TArray<AActor*> TargetActors;
	
	UPROPERTY()
	TObjectPtr<AActor> ClosestActorToMouse;

	UFUNCTION()
	void OnRep_ClosestActorToMouse();

	UPROPERTY()
	TObjectPtr<UUserWidget> TargetLockWidget;
	
};
