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
	 * 클라이언트는 설정이 안되있기 때문에 게임모드에서 Login 감지하면 Client PRC로 TargetLevelPath 보내줌.
	 * 클라이언트 RPC로 TargetLevelPath가 설정 완료되면 클라이언트에서 직접 StartAsyncLoading() 호출.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void StartAsyncLoading();
	
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelMap() const;

	/* 트랜지션 맵으로 이동하는 함수로 NM_Standalone일 경우 Open Level을 NM_ListenServer일 경우 ServerTravel을 실행 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelToTransitionMap(const TSoftObjectPtr<UWorld> TransitionMapClass);
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FAsyncLoadProgressDelegate OnAsyncLoadingUpdateDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FOnAsyncLoadingCompleteDelegate OnAsyncLoadingCompleteDelegate;

	/* 맵 로딩이 완료된 플레이어를 등록 */
	void RegisterPlayerLoaded(const APlayerController* NewPlayer);
	/* 클라이언트가 로딩 중 게임을 나가면 로드 완료된 플레이어 Set에서 제외 */
	void UnregisterPlayerLoaded(const APlayerController* ExitingPlayer);

private:
	/* 이동할 맵의 에셋 네임 */
	FName TargetLevelPath;
	
	void OnLoadPackageCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);
	void OnLoadPackageUpdated();
	FTimerHandle LoadingPercentTimerHandle;
	
	/* 맵 로딩이 완료된 플레이어 고유 아이디를 저장하는 Set */
	UPROPERTY()
	TSet<const APlayerController*> LoadedPlayerControllerSet;

	void CheckAllPlayersLoaded();
	

public:
	FORCEINLINE FName GetTraveledLevelPath() const { return TargetLevelPath; }
	
};
