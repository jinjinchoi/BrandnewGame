// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Sequnce/SequenceManager.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Engine/AssetManager.h"

void USequenceManager::PlayFirstEntranceSequence()
{
	if (!GetWorld() ||FirstEntranceSequence.IsNull()) return;

	TWeakObjectPtr WeakThis = this;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(FirstEntranceSequence.ToSoftObjectPath(), [WeakThis]()
	{
		if (!WeakThis.IsValid()) return;

		ULevelSequence* LoadedSequence = WeakThis->FirstEntranceSequence.Get();
		if (!LoadedSequence) return;

		APlayerController* PC = WeakThis->GetWorld()->GetFirstPlayerController();
		if (!PC) return;
		PC->SetCinematicMode(true, true, true, true, true);

		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* OutActor = nullptr;

		ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
			WeakThis->GetWorld(),LoadedSequence, Settings, OutActor);

		if (Player)
		{
			Player->Play();
			WeakThis->OnSequencePlayStateChangedDelegate.Broadcast(true);
			Player->OnFinished.AddUniqueDynamic(WeakThis.Get(), &ThisClass::OnCinematicFinishedPlaying);
		}

	
		
	});
}

void USequenceManager::OnCinematicFinishedPlaying()
{
	OnSequencePlayStateChangedDelegate.Broadcast(false);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	PC->SetCinematicMode(false, true, true, true, true);
	if (APawn* Pawn = PC->GetPawn())
	{
		PC->SetViewTarget(Pawn);
	}
}
