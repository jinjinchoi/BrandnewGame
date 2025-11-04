// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BrandNewAIController.generated.h"

class UAISenseConfig_Damage;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	ABrandNewAIController();
	
	/* Begin IGenericTeamAgentInterface Interface.*/
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	/* End IGenericTeamAgentInterface Interface. */

protected:
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* DetectedActor, FAIStimulus Stimulus);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Sight> AISenseConfig_Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAISenseConfig_Damage> AISenseConfig_Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|AI Perception")
	float LostTargetDelay = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|AI Perception", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float TargetChangeChance = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|AI Perception")
	float TargetChangeCooldown = 10.0f;

private:
	void HandleLostTarget(AActor* LostActor);
	FTimerHandle LostTargetTimer;
	FDelegateHandle CharacterDiedDelegateHandle;

	void SetNewTarget(AActor* NewTarget);

	FTimerHandle TargetChangeCooldownTimer;
	bool bCanChangeTarget = true;
	
};
