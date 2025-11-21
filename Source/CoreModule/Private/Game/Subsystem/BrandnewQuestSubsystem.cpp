// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandnewQuestSubsystem.h"

void UBrandnewQuestSubsystem::AddQuestActorToMap(const FName& ActorId, AActor* Actor)
{
	if (QuestActorMap.Contains(ActorId)) return;
	
	QuestActorMap.Add(ActorId, Actor);
	
	if (PendingActorId != NAME_None && PendingActorId == ActorId)
	{
		OnQuestActorSetDelegate.Broadcast();
		PendingActorId = NAME_None;
	}
	
}

void UBrandnewQuestSubsystem::ClearSubsystem()
{
	QuestActorMap.Empty();
}

AActor* UBrandnewQuestSubsystem::GetQuestTargetById(const FName& ActorId)
{
	if (QuestActorMap.Contains(ActorId))
	{
		PendingActorId = NAME_None;
		return QuestActorMap[ActorId];
	}
	
	PendingActorId = ActorId;
	return nullptr;
}
