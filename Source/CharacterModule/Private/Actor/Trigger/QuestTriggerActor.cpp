// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Trigger/QuestTriggerActor.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"


AQuestTriggerActor::AQuestTriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	SetRootComponent(BoxCollision);
	
	LocationWidgetComponent->SetupAttachment(GetRootComponent());
	
}

void AQuestTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	
}

void AQuestTriggerActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
	
	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(OtherActor);
	if (!PlayerInterface) return;
	
	// Authority나 Local Controller는 해당 함수에서 확인하고 있음.
	PlayerInterface->IncreaseQuestProgressById(ActorId);
	PlayerInterface->TryStartQuestDialogue(ActorId);
	
}