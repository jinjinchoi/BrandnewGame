// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BrandNewLevelManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncLoadProgressDelegate, const float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncLoadingCompleteDelegate, const bool, bSucceeded);

/**
 * 
 */
UCLASS()
class COREMODULE_API UBrandNewLevelManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void SetMapNameToTravel(const TSoftObjectPtr<UWorld> LevelClass);

	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void SetMapNameToTravelByString(const FString& MapName);

	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void StartAsyncLoading();

	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelMap() const; 
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FAsyncLoadProgressDelegate OnAsyncLoadingUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FOnAsyncLoadingCompleteDelegate OnAsyncLoadingCompleteDelegate;

private:
	void OnLoadPackageCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);
	void OnLoadPackageUpdated();
	
	FName TargetLevelPath;
	FTimerHandle LoadingPercentTimerHandle;
};
