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
	 * TODO: 차후 맵 에셋들을 로드할때 사용할 함수.
	 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void StartAsyncLoading();
	
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelMap() const;

	/* 트랜지션 맵으로 이동하는 함수로 NM_Standalone일 경우 Open Level을 NM_ListenServer일 경우 ServerTravel을 실행 */
	UFUNCTION(BlueprintCallable, Category = "Brandnew|SubystemFunction")
	void TravelToTransitionMap(const TSoftObjectPtr<UWorld> TransitionMapClass);

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegate")
	FOnAsyncLoadingCompleteDelegate OnAsyncLoadingCompleteDelegate;

	/* 맵 로딩이 완료된 플레이어를 등록 */
	void RegisterPlayerLoaded(const APlayerController* NewPlayer);
	/* 클라이언트가 로딩 중 게임을 나가면 로드 완료된 플레이어 Set에서 제외 */
	void UnregisterPlayerLoaded(const APlayerController* ExitingPlayer);

	UFUNCTION(BlueprintCallable, Category = "Brandnew|Subsystem Function")
	void ResetLevelManagerSubsystem();

private:
	/* 이동할 맵의 에셋 네임 */
	FName TargetLevelPath = NAME_None;
	
	/* 맵 로딩이 완료된 플레이어 고유 아이디를 저장하는 Set */
	UPROPERTY()
	TSet<const APlayerController*> LoadedPlayerControllerSet;

	void CheckAllPlayersLoaded();

public:
	UFUNCTION(BlueprintPure, Category = "Brandnew|Subsystem Function")
	FORCEINLINE FName GetTraveledLevelPath() const { return TargetLevelPath; }
	
};
