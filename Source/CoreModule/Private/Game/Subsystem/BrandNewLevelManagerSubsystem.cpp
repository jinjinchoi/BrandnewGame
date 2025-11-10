// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"
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
	
	TargetLevelPath = FName(*MapName);
	
}


void UBrandNewLevelManagerSubsystem::StartAsyncLoading()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return; // 서버는 로딩하지 않음
	}

	// TODO: 실제로 에셋 로드 구현하고 성공하면 브로드캐스트 해야함
	OnAsyncLoadingCompleteDelegate.Broadcast(true);
	
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

void UBrandNewLevelManagerSubsystem::ResetLevelManagerSubsystem()
{
	LoadedPlayerControllerSet.Empty();
	TargetLevelPath = NAME_None;
	
}