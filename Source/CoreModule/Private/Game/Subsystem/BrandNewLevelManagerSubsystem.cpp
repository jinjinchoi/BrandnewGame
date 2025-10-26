// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"

#include "DebugHelper.h"
#include "Kismet/GameplayStatics.h"

void UBrandNewLevelManagerSubsystem::SetMapNameToTravel(const TSoftObjectPtr<UWorld> LevelClass)
{
	if (LevelClass.IsNull()) return;
	
	const FSoftObjectPath& SoftObjectPath = LevelClass.ToSoftObjectPath();
	FString PackageName = SoftObjectPath.GetLongPackageName();

#if WITH_EDITOR
	if (const UWorld* World = GWorld)
	{
		if (GIsEditor && World->IsPlayInEditor())
		{
			const FString Prefix = World->StreamingLevelsPrefix;
			PackageName = UWorld::StripPIEPrefixFromPackageName(PackageName, Prefix);
		}
	}
#endif

	const FName PackageFName(*PackageName);
	TargetLevelPath = PackageFName;
}

void UBrandNewLevelManagerSubsystem::SetMapNameToTravelByString(const FString& MapName)
{
	if (MapName.IsEmpty()) return;

	// TODO: 맵 에셋 네임 클라이언트 전부에게 설정해야함
	
	TargetLevelPath = FName(*MapName);
}


void UBrandNewLevelManagerSubsystem::StartAsyncLoading()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return; // 서버는 로딩하지 않음
	}
	
	if (TargetLevelPath == NAME_None)
	{
		const FString DebugString = FString::Printf(TEXT("%hs 맵 에셋 네임이 올바르게 설정되지 않았습니다."), __FUNCTION__);
		DebugHelper::Print(DebugString, FColor::Red);
		return;
	}
	
	LoadPackageAsync(
		TargetLevelPath.ToString(),
		FLoadPackageAsyncDelegate::CreateUObject(this, &UBrandNewLevelManagerSubsystem::OnLoadPackageCompleted),
		0,
		PKG_ContainsMap);

	
	// Unreal 5.5 이상 버전에서는 GetAsyncLoadPercentage 호출해도 정상적인 값을 받을 수 없다고 함.
	// GetWorld()->GetTimerManager().SetTimer(LoadingPercentTimerHandle, this, &ThisClass::OnLoadPackageUpdated, 0.2f,true, 0.2f);
}


void UBrandNewLevelManagerSubsystem::TravelMap() const
{
	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TargetLevelPath);
	}
	else
	{
		const FString TravelURL = FString::Printf(TEXT("%s?listen"), *TargetLevelPath.ToString());
		GetWorld()->ServerTravel(TravelURL);
	}
}

void UBrandNewLevelManagerSubsystem::OnLoadPackageCompleted(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	GetWorld()->GetTimerManager().ClearTimer(LoadingPercentTimerHandle);
	if (Result == EAsyncLoadingResult::Succeeded)
	{
		OnAsyncLoadingCompleteDelegate.Broadcast(true);
	}
	else
	{
		OnAsyncLoadingCompleteDelegate.Broadcast(false);
	}
}

void UBrandNewLevelManagerSubsystem::OnLoadPackageUpdated()
{
	float LoadPercentage = GetAsyncLoadPercentage(TargetLevelPath);
	OnAsyncLoadingUpdateDelegate.Broadcast(LoadPercentage);
}

void UBrandNewLevelManagerSubsystem::TravelToTransitionMap(const TSoftObjectPtr<UWorld> TransitionMapClass)
{
	const FSoftObjectPath& SoftObjectPath = TransitionMapClass.ToSoftObjectPath();
	const FString PackageString = SoftObjectPath.GetLongPackageName();

	if (GetWorld()->GetNetMode() == NM_Standalone)
	{
		const FName PackageFName(*PackageString);
		UGameplayStatics::OpenLevel(GetWorld(), PackageFName);
	}
	else
	{
		const FString TravelURL = FString::Printf(TEXT("%s?listen"), *PackageString);
		GetWorld()->ServerTravel(TravelURL);
	}
}

void UBrandNewLevelManagerSubsystem::RegisterPlayerLoaded(const APlayerController* NewPlayer)
{
	LoadedPlayerControllerSet.Add(NewPlayer);
	CheckAllPlayersLoaded();
	
}

void UBrandNewLevelManagerSubsystem::UnregisterPlayerLoaded(const APlayerController* ExitingPlayer)
{
	LoadedPlayerControllerSet.Remove(ExitingPlayer);
	CheckAllPlayersLoaded();
}

void UBrandNewLevelManagerSubsystem::CheckAllPlayersLoaded()
{
	if (LoadedPlayerControllerSet.Num() >= GetWorld()->GetNumPlayerControllers())
	{
		// 이동 작업
		LoadedPlayerControllerSet.Empty();
		TravelMap();
	}
}
