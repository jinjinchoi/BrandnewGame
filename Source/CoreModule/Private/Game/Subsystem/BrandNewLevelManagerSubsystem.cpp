// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"

#include "DebugHelper.h"
#include "Kismet/GameplayStatics.h"

void UBrandNewLevelManagerSubsystem::SetMapNameToTravel(const TSoftObjectPtr<UWorld> LevelClass)
{
	if (LevelClass.IsNull()) return;
	
	const FSoftObjectPath& SoftObjectPath = LevelClass.ToSoftObjectPath();
	const FString PackageName = SoftObjectPath.GetLongPackageName();
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
	
	if (TargetLevelPath == NAME_None)
	{
		DebugHelper::Print(TEXT("맵 에셋 네임이 올바르게 설정되지 않았습니다."), FColor::Red);
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
	UGameplayStatics::OpenLevel(GetWorld(), TargetLevelPath);
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
