// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandnewQuestSubsystem.h"

void UBrandnewQuestSubsystem::AddQuestActorToMap(const FName& ActorId, AActor* Actor)
{
	if (QuestActorMap.Contains(ActorId)) return;
	
	QuestActorMap.Add(ActorId, Actor);
	
}

void UBrandnewQuestSubsystem::RemoveQuestActorFromMap(const FName& ActorId)
{
	if (QuestActorMap.Contains(ActorId))
	{
		QuestActorMap.Remove(ActorId);
	}
}

AActor* UBrandnewQuestSubsystem::GetActorFromMap(const FName& ActorId)
{
	if (QuestActorMap.Contains(ActorId))
	{
		return QuestActorMap[ActorId];
	}
	
	return nullptr;
}
