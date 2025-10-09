// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BrandNewGameModeBase.h"

#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "Manager/Pooling/BrandNewObjectPoolManager.h"

void ABrandNewGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	check(ObjectPoolManagerClass);

	if (!ObjectPoolManager)
	{
		ObjectPoolManager = NewObject<UBrandNewObjectPoolManager>(this, ObjectPoolManagerClass);
	}

	ObjectPoolManager->InitPoolManager();
	
}

void ABrandNewGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	/**
	 * New Player가 생성되면 서브시스템에서 네임 가져와서 Player State에 설정
	 * Player Name은 복제가 되기 때문에 서버에서만 실행하면 됨.
	 * 현재 Player Name 복제 시기 여부를 OnRep_PlayerId 에서 하고 있는데 시점이 맞는지 패키징 후에도 확인해아함.
	 */
	UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;

	APlayerState* PS = NewPlayer->GetPlayerState<APlayerState>();
	if (!PS) return;
	IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(PS);
	if (!PlayerStateInterface) return;
	PlayerStateInterface->SetPlayerNameToPlayerState(SaveSubsystem->GetUniqueIdentifier());
	
}
