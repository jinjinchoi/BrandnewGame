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
	/* 맵 이동 전 맵 클래스를 넘겨 이동할 맵을 설정 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void SetMapNameToTravel(const TSoftObjectPtr<UWorld> LevelClass);

	/* 로드 시 맵 에셋 네임을 넘겨 이동할 맵을 설정 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void SetMapNameToTravelByString(const FString& MapName);

	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void StartAsyncLoading();

	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelMap() const;

	/**
	 * 전환 맵으로 이동하는 함수로 NM_Standalone일 경우 Open Level을 NM_ListenServer일 경우 ServerTravel을 실행
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelToTransitionMap(const TSoftObjectPtr<UWorld> TransitionMapClass);
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FAsyncLoadProgressDelegate OnAsyncLoadingUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FOnAsyncLoadingCompleteDelegate OnAsyncLoadingCompleteDelegate;

private:
	void OnLoadPackageCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);
	void OnLoadPackageUpdated();

	/* 이동할 맵의 에셋 네임 */
	FName TargetLevelPath;
	FTimerHandle LoadingPercentTimerHandle;
};
