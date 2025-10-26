// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/BrandNewGameState.h"
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
	Super::RemovePlayerState(PlayerState);
	
	PlayerExitDelegate.Broadcast(PlayerState);
	
}