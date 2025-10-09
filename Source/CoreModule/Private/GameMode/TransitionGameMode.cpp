// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TransitionGameMode.h"

void ATransitionGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	ErrorMessage = TEXT("Server is loading. Please try again later.");
	
}
