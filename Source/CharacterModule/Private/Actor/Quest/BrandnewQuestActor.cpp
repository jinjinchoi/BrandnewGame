// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Quest/BrandnewQuestActor.h"
#include "Components/WidgetComponent.h"
#include "Game/Subsystem/BrandnewQuestSubsystem.h"


ABrandnewQuestActor::ABrandnewQuestActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	LocationWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Location Widget Component"));
	LocationWidgetComponent->SetupAttachment(GetRootComponent());
	LocationWidgetComponent->SetVisibility(false);

}

void ABrandnewQuestActor::BeginPlay()
{
	Super::BeginPlay();
	
	check(ActorId.IsValid());
	
	if (UBrandnewQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UBrandnewQuestSubsystem>())
	{
		QuestSubsystem->AddQuestActorToMap(ActorId, this);
	}
	

}

void ABrandnewQuestActor::ShowLocationWidget(const bool bIsVisible)
{
	LocationWidgetComponent->SetVisibility(bIsVisible);
}

FName ABrandnewQuestActor::GetQuestActorId() const
{
	return ActorId;
}
