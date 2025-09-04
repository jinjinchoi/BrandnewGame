// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BrandNewAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"

ABrandNewAIController::ABrandNewAIController()
{
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 1500.f;
	AISenseConfig_Sight->LoseSightRadius = 2500.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 60.f;
	
	AISenseConfig_Damage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AISenseConfig_Damage"));

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Damage);
	EnemyPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	AAIController::SetGenericTeamId(FGenericTeamId(TEAM_ENEMY));
}

ETeamAttitude::Type ABrandNewAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void ABrandNewAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	if (!IsValid(Actor))
	{
		BlackboardComponent->ClearValue("TargetActor");
		GetWorldTimerManager().ClearTimer(LostTargetTimer);
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(LostTargetTimer);
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
	else
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::HandleLostTarget, Actor);
		
		GetWorldTimerManager().SetTimer(LostTargetTimer, TimerDelegate, LostTargetDelay, false);
		
	}
}

void ABrandNewAIController::HandleLostTarget(AActor* LostActor)
{
	if (!IsValid(LostActor)) return;
	
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	UObject* CurrentTarget = BlackboardComponent->GetValueAsObject("TargetActor");
	if (IsValid(CurrentTarget) && CurrentTarget == LostActor)
	{
		BlackboardComponent->ClearValue("TargetActor");
	}
}
