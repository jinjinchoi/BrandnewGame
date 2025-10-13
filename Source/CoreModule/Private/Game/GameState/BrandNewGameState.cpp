// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/BrandNewGameState.h"

#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"

void ABrandNewGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	
	IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(PlayerState);
	if (!PlayerStateInterface) return;

	// Player State 초기화 여부 확인하고 되었으면 UI에 알려서 같이 하는 플레이어들 체력바 UI에 표시함.
	if (PlayerStateInterface->IsPlayerReplicated())
	{
		PlayerJoinDelegate.Broadcast(PlayerState);
	}
	else
	{
		TWeakObjectPtr WeakThis = this;
		TWeakObjectPtr WeakPlayerState = PlayerState;
		
		PlayerStateInterface->GetPlayerSetDelegate().BindLambda([WeakThis, WeakPlayerState]()
		{
			if (WeakThis.IsValid() && WeakPlayerState.IsValid())
			{
				WeakThis->PlayerJoinDelegate.Broadcast(WeakPlayerState.Get());
			}
		});
	}

	
}

void ABrandNewGameState::RemovePlayerState(APlayerState* PlayerState)
{
	
	PlayerExitDelegate.Broadcast(PlayerState);

	// 로딩 중 플레이어가 나가면 Set에서 제외하고 체크
	if (!LoadedPlayerIdSet.IsEmpty())
	{
		LoadedPlayerIdSet.Remove(PlayerState->GetPlayerId());
		CheckAllPlayersLoaded();
	}
	
	Super::RemovePlayerState(PlayerState);

}

void ABrandNewGameState::RegisterPlayerLoaded(const int32 PlayerId)
{
	LoadedPlayerIdSet.Add(PlayerId);
	
	CheckAllPlayersLoaded();
}


void ABrandNewGameState::CheckAllPlayersLoaded()
{
	if (!HasAuthority()) return;
	
	if (LoadedPlayerIdSet.Num() >= PlayerArray.Num())
	{
		// 이동 작업
		LoadedPlayerIdSet.Empty();
		
		const UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>();
		check(LevelManagerSubsystem);
		LevelManagerSubsystem->TravelMap();
		
	}
}

