// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Map/MapEntrance.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/GameState/BrandNewGameState.h"
#include "Game/Subsystem/BrandNewLevelManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "Interfaces/Player/BnPlayerControllerInterface.h"

// Sets default values
AMapEntrance::AMapEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(BoxCollision);

	BoxCollision->SetBoxExtent(FVector(500.f, 500.f, 200.f));

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget Component"));
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	InteractionWidgetComponent->SetVisibility(false);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
}

// Called when the game starts or when spawned
void AMapEntrance::BeginPlay()
{
	Super::BeginPlay();
	
	check(!LevelToTravelClass.IsNull() || !TransitionLevelClass.IsNull());
	if (LevelToTravelClass.IsNull() || TransitionLevelClass.IsNull()) return;

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
	
	ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState());
	check(BrandNewGameState);
	BrandNewGameState->PlayerJoinDelegate.AddDynamic(this, &ThisClass::OnPlayerJoined);
	BrandNewGameState->PlayerExitDelegate.AddDynamic(this, &ThisClass::OnPlayerExited);
	
}

void AMapEntrance::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
	
	if (OtherActor->Implements<UBrandNewPlayerInterface>() && !OtherActor->IsPendingKillPending() && !OtherActor->IsActorBeingDestroyed())
	{
		OverlappingActors.Add(OtherActor);
		CheckAllPlayersOverlapped();
		CreateOrUpdateEntryStatusWidget();
	}

	if (const ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (Player->IsLocallyControlled())
		{
			InteractionWidgetComponent->SetVisibility(true);
		}
	}
	
}

void AMapEntrance::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
	
	if (OverlappingActors.Contains(OtherActor))
	{
		OverlappingActors.Remove(TWeakObjectPtr<AActor>(OtherActor));
		CreateOrUpdateEntryStatusWidget();
	}

	if (const ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (Player->IsLocallyControlled())
		{
			InteractionWidgetComponent->SetVisibility(false);
		}
	}
	
}


void AMapEntrance::OnPlayerJoined(const APlayerState* NewPlayerState)
{
	if (OverlappingActors.Num() > 0)
	{
		CreateOrUpdateEntryStatusWidget();
		CheckAllPlayersOverlapped();
	}
	
}

void AMapEntrance::OnPlayerExited(const APlayerState* ExitedPlayerState)
{
	if (!ExitedPlayerState) return;
	
	if (OverlappingActors.Contains(ExitedPlayerState->GetPawn()))
	{
		OverlappingActors.Remove(ExitedPlayerState->GetPawn());
	}
	
	if (OverlappingActors.Num() > 0)
	{
		CreateOrUpdateEntryStatusWidget();
		CheckAllPlayersOverlapped();
	}
}

void AMapEntrance::CleanupInvalidActors()
{
	for (auto It = OverlappingActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}

void AMapEntrance::CheckAllPlayersOverlapped()
{
	CleanupInvalidActors();

	if (!HasAuthority()) return;

	const ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState());
	if (!BrandNewGameState) return;
	
	// 오버랩 된 액터와 플레이어의 수가 같으면 맵 이동
	if (OverlappingActors.Num() == BrandNewGameState->PlayerArray.Num())
	{
		UBrandNewLevelManagerSubsystem* LevelManagerSubsystem = GetGameInstance()->GetSubsystem<UBrandNewLevelManagerSubsystem>();
		if (!LevelManagerSubsystem) return;

		for (TWeakObjectPtr<AActor>& WeakActor : OverlappingActors)
		{
			if (AActor* Actor = WeakActor.Get())
			{
				if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(Actor))
				{
					// 맵 이동 전 현재 데이터 저장
					PlayerInterface->SavePlayerDataForTravel();
				}
				
			}
		}

		LevelManagerSubsystem->SetMapNameToTravel(LevelToTravelClass);
		LevelManagerSubsystem->TravelToTransitionMap(TransitionLevelClass);
	}
	
}

void AMapEntrance::CreateOrUpdateEntryStatusWidget() const
{
	const ABrandNewGameState* BrandNewGameState = Cast<ABrandNewGameState>(GetWorld()->GetGameState());
	if (!BrandNewGameState) return;

	const int32 OverlappedPlayerCount = OverlappingActors.Num();
	const int32 MaxPlayersCount = BrandNewGameState->PlayerArray.Num();
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (!IsValid(PlayerController) || !PlayerController->IsLocalController()) continue;

		IBnPlayerControllerInterface* PlayerControllerInterface = Cast<IBnPlayerControllerInterface>(PlayerController);
		if (!PlayerControllerInterface) continue;

		PlayerControllerInterface->HandlePlayerMapEntryOverlap(OverlappedPlayerCount, MaxPlayersCount);
		
	}
	
}

