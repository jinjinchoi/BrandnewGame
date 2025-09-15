// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BrandNewGameModeBase.generated.h"

class UBrandNewObjectPoolManager;
/**
 * 
 */
UCLASS()
class COREMODULE_API ABrandNewGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Manager")
	TSubclassOf<UBrandNewObjectPoolManager> ObjectPoolManagerClass;

	UPROPERTY()
	TObjectPtr<UBrandNewObjectPoolManager> ObjectPoolManager;

public:
	FORCEINLINE UBrandNewObjectPoolManager* GetObjectPoolManager() const { return ObjectPoolManager; };
	
};
