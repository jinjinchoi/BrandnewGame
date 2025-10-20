// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SequenceManager.generated.h"

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
	void PlayFirstEntranceSequence();

	FOnSequencePlayStateChanged OnSequencePlayStateChangedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cinematics")
	TSoftObjectPtr<ULevelSequence> FirstEntranceSequence;

private:
	UFUNCTION()
	void OnCinematicFinishedPlaying();
	
};
