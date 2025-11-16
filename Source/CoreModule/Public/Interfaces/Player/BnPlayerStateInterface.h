// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "UObject/Interface.h"
#include "BnPlayerStateInterface.generated.h"


// This class does not need to be modified.
UINTERFACE()
class UBnPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBnPlayerStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FInventoryContents GetInventoryContents() const = 0;
	virtual FString GetPlayerUniqueId() const = 0;
	/* Core 모듈은 Quest 모듈 종속성이 없어 Quest Component를 바로 반환하지 못함. 따라서 Cast 해서 사용해야함. */
	virtual UActorComponent* GetQuestComponent() const = 0;
	
};
