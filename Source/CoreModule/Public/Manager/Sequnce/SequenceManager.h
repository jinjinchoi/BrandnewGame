// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SequenceManager.generated.h"

class ULevelSequencePlayer;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSequencePlayStateChanged, const bool /* Is Playing? */);

class ULevelSequence;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COREMODULE_API USequenceManager : public UObject
{
	GENERATED_BODY()

public:
	void PlayFirstEntranceSequence() const;
	void PlayDialogueSequence(const TSoftObjectPtr<ULevelSequence> SequenceToPlay);
	void FinishDialogueSequence();

	FOnSequencePlayStateChanged OnSequencePlayStateChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Cinematics")
	TSoftObjectPtr<ULevelSequence> FirstEntranceSequence;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Cinematics")
	bool bShouldPlayEntranceSequenceInEditor = false;

private:
	UFUNCTION()
	void OnCinematicFinishedPlaying();
	
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LastSequencePlayer;
	
};
