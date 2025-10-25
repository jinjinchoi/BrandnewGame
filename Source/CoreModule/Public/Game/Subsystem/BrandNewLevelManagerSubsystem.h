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

	/**
	 * Level을 비동기 로드하는 함수로 서버에서 서버의 경우 이 함수를 실행할때는 이미 TargetLevelPath가 설정되어 있지만
	 * 클라이언트는 설정이 안되있기 때문에 클라이언트는 RequestAsyncLoadToAllClient() 함수를 사용해서 TargetLevelPath를 설정해야함.
	 * 클라이언트 RPC로 TargetLevelPath가 설정 완료되면 클라이언트에서 직접 StartAsyncLoading() 호출.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void StartAsyncLoading();

	/**
	 * 서버에서만 실행시켜야하는 함수로 클라이언트에 로딩할 Map Path를 알려주는 함수.
	 * 다만 트랜지션 맵에서 해당 함수를 실행시키는데 이때 아직 클라이언트들이 서버에 접속을 안해있을 가능성이 매우 높고
	 * 그렇기 때문에 GameMode의 Post Login 함수에서 새로 들어온 클라이언트에게 Map Path를 알려주도록 구현.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void RequestAsyncLoadToAllClient() const;

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

public:
	FORCEINLINE FName GetTraveledLevelPath() const { return TargetLevelPath; }
	
};
