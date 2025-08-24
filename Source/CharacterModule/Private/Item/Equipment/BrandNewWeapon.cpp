// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/BrandNewWeapon.h"

#include "Components/BoxComponent.h"

ABrandNewWeapon::ABrandNewWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision Box"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));

	SetReplicates(true);

}

void ABrandNewWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}