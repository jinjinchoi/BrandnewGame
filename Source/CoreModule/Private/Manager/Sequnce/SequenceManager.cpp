// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Sequnce/SequenceManager.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Engine/AssetManager.h"
#include "Interfaces/UI/BrandNewHUDInterface.h"
#include "GameFramework/HUD.h"

void USequenceManager::PlayFirstEntranceSequence()
{
	if (GIsPlayInEditorWorld && !bShouldPlayEntranceSequenceInEditor)
	{
		return;
	}
	
	if (!GetWorld() || FirstEntranceSequence.IsNull()) return;

	TWeakObjectPtr WeakThis = this;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(FirstEntranceSequence.ToSoftObjectPath(), [WeakThis]()
	{
		if (!WeakThis.IsValid()) return;

		ULevelSequence* LoadedSequence = WeakThis->FirstEntranceSequence.Get();
		if (!LoadedSequence) return;
		
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bDisableLookAtInput = true;
		Settings.bDisableMovementInput = true;
		Settings.bHidePlayer = true;
		
		ALevelSequenceActor* OutActor = nullptr;

		ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
			WeakThis->GetWorld(),LoadedSequence, Settings, OutActor);

		if (Player)
		{
			const APlayerController* PC = WeakThis->GetWorld()->GetFirstPlayerController();
			
			IBrandNewHUDInterface* HUDInterface = Cast<IBrandNewHUDInterface>(PC->GetHUD());
			if (!HUDInterface) return;
			HUDInterface->CreateSequenceOverlayWidget();
			
			Player->Play();
			WeakThis->OnSequencePlayStateChangedDelegate.Broadcast(true);
			Player->OnFinished.AddUniqueDynamic(WeakThis.Get(), &ThisClass::OnCinematicFinishedPlaying);
			WeakThis->LastSequencePlayer = Player;
		}
		
	});
}

void USequenceManager::OnCinematicFinishedPlaying()
{
	OnSequencePlayStateChangedDelegate.Broadcast(false);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	
	IBrandNewHUDInterface* HUDInterface = Cast<IBrandNewHUDInterface>(PC->GetHUD());
	if (!HUDInterface) return;
	
	HUDInterface->RemoveSequenceOverlayWidget();
	
	if (APawn* Pawn = PC->GetPawn())
	{
		PC->SetViewTarget(Pawn);
	}
}


void USequenceManager::PlayDialogueSequence(const TSoftObjectPtr<ULevelSequence> SequenceToPlay)
{
	if (SequenceToPlay.IsNull()) return;
	
	TWeakObjectPtr WeakThis = this;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(SequenceToPlay.ToSoftObjectPath(), [WeakThis, SequenceToPlay]()
	{
		if (!WeakThis.IsValid()) return;
		
		ULevelSequence* LoadedSequence = SequenceToPlay.Get();
		ALevelSequenceActor* OutActor = nullptr;
		
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bDisableLookAtInput = true;
		Settings.bDisableMovementInput = true;
		Settings.bHidePlayer = true;
		Settings.bPauseAtEnd = true;
		
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			WeakThis->GetWorld(),LoadedSequence, Settings, OutActor);

		if (SequencePlayer)
		{
			APlayerController* PC = WeakThis->GetWorld()->GetFirstPlayerController();
			PC->DisableInput(PC);
			
			SequencePlayer->Play();

			if (WeakThis->LastSequencePlayer)
			{
				WeakThis->LastSequencePlayer->Stop();
			}
			
			WeakThis->LastSequencePlayer = SequencePlayer;
		}
		
	});
	
}

void USequenceManager::StopCurrentSequence()
{
	if (LastSequencePlayer)
	{
		LastSequencePlayer->Stop();
		LastSequencePlayer = nullptr;
		
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (APawn* Pawn = PC->GetPawn())
		{
			PC->SetViewTarget(Pawn);
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [PC]()
			{
				PC->EnableInput(PC);
			}, 0.5f, false);
			
		}
	}
	
}