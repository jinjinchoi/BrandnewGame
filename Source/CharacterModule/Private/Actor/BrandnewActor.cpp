// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BrandnewActor.h"
#include "Components/WidgetComponent.h"
#include "Game/Subsystem/BrandnewQuestSubsystem.h"


ABrandnewActor::ABrandnewActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	LocationWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Location Widget Component"));
	LocationWidgetComponent->SetupAttachment(GetRootComponent());
	LocationWidgetComponent->SetVisibility(false);

}

void ABrandnewActor::BeginPlay()
{
	Super::BeginPlay();
	
	check(ActorId.IsValid());
	
	if (UBrandnewQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UBrandnewQuestSubsystem>())
	{
		QuestSubsystem->AddQuestActorToMap(ActorId, this);
	}
	

}

void ABrandnewActor::Destroyed()
{
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		if (UBrandnewQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UBrandnewQuestSubsystem>())
		{
			QuestSubsystem->RemoveQuestActorFromMap(ActorId);
		}
	}
    
	Super::Destroyed();
}

void ABrandnewActor::ShowLocationWidget(const bool bIsVisible)
{
	LocationWidgetComponent->SetVisibility(bIsVisible);
}