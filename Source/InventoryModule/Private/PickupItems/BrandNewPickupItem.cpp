// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItems/BrandNewPickupItem.h"

#include "Components/SphereComponent.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "NiagaraComponent.h"

// Sets default values
ABrandNewPickupItem::ABrandNewPickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Item Effect"));
	NiagaraComponent->SetupAttachment(GetRootComponent());
	NiagaraComponent->bAutoActivate = true;

	ItemUniqueId = FGuid::NewGuid();
	
}

void ABrandNewPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereEndOverlap);
	
}

void ABrandNewPickupItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ItemId == INDEX_NONE) return;
	
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(OtherActor))
	{
		PlayerInterface->AddOverlappedItem(this);
	}
	
}

void ABrandNewPickupItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ItemId == INDEX_NONE) return;
	
	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(OtherActor))
	{
		PlayerInterface->RemoveOverlappedItem(this);
	}
}



int32 ABrandNewPickupItem::GetId() const
{
	return ItemId;
}

int32 ABrandNewPickupItem::GetQuantity()
{
	return Quantity;
}

FGuid ABrandNewPickupItem::GetUniqueId()
{
	return ItemUniqueId;
}