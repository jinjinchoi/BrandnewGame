// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BrandNewAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "Interfaces/Character/BrandNewCharacterInterface.h"
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
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (!OtherPawn) return ETeamAttitude::Neutral;

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (!OtherTeamAgent) return ETeamAttitude::Neutral;

	if (!OtherPawn->Implements<UBrandNewCharacterInterface>() || IBrandNewCharacterInterface::Execute_IsDead(OtherPawn))
	{
		return ETeamAttitude::Neutral;
	}
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void ABrandNewAIController::OnEnemyPerceptionUpdated(AActor* DetectedActor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	IBrandNewCharacterInterface* CharacterInterface = Cast<IBrandNewCharacterInterface>(DetectedActor);
	if (!CharacterInterface) return;
	
	if (Stimulus.WasSuccessfullySensed())
	{
		GetWorld()->GetTimerManager().ClearTimer(LostTargetTimer);

		// 에너미에 타겟이 설정되어있지 않을때만 타겟 설정
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor")))
		{
			if (!CharacterInterface->GetOnCharacterDiedDelegate().IsBoundToObject(this))
			{
				CharacterDiedDelegateHandle = CharacterInterface->GetOnCharacterDiedDelegate().AddWeakLambda(this, [this, BlackboardComponent]()
				{
					if (!IsValid(BlackboardComponent)) return;
					BlackboardComponent->ClearValue("TargetActor");
				});
			}
			
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), DetectedActor);
		}
	}
	else
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::HandleLostTarget, DetectedActor);
		
		GetWorldTimerManager().SetTimer(LostTargetTimer, TimerDelegate, LostTargetDelay, false);
		
	}
}

void ABrandNewAIController::HandleLostTarget(AActor* LostActor)
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;
	
	if (!IsValid(LostActor))
	{
		BlackboardComponent->ClearValue("TargetActor");
		return;
	}
	
	UObject* CurrentTarget = BlackboardComponent->GetValueAsObject("TargetActor");
	if (IsValid(CurrentTarget) && CurrentTarget == LostActor)
	{
		if (CharacterDiedDelegateHandle.IsValid())
		{
			IBrandNewCharacterInterface* CharacterInterface = Cast<IBrandNewCharacterInterface>(LostActor);
			CharacterInterface->GetOnCharacterDiedDelegate().Remove(CharacterDiedDelegateHandle);
			CharacterDiedDelegateHandle.Reset();
		}
		
		BlackboardComponent->ClearValue("TargetActor");
	}
}

