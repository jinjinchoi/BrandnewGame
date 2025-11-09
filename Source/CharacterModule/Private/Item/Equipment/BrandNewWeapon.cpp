// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/BrandNewWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CharacterFunctionLibrary.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "Components/BoxComponent.h"
#include "Interfaces/Character/BrandNewCharacterInterface.h"

ABrandNewWeapon::ABrandNewWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(WeaponMesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision Box"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->SetCollisionResponseToChannel(ECC_Ground, ECR_Ignore);
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));

	bReplicates = true;

}

void ABrandNewWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::CollisionBoxBeginOverlap);
	}
	
}

void ABrandNewWeapon::ToggleCollisionEnable(const bool bIsEnable)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	
	if (bIsEnable)
	{
		WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
	}
}

void ABrandNewWeapon::SetWeaponVisible(const bool bIsVisible)
{
	SetActorHiddenInGame(!bIsVisible);
}

void ABrandNewWeapon::CollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bIsNotValid =
		!IsValid(OtherActor) ||
		!OtherActor->Implements<UBrandNewCharacterInterface>() ||
		OverlappedActors.Contains(OtherActor) ||
		!GetOwner() ||
		GetOwner() == OtherActor;

	if (bIsNotValid) return;

	if (!UCharacterFunctionLibrary::IsTargetActorHostile(GetOwner(), OtherActor))
	{
		return;
	}

	OverlappedActors.Add(OtherActor);

	FGameplayEventData Data;
	Data.Instigator = GetInstigator();
	Data.Target = OtherActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), BrandNewGamePlayTag::Event_Hit_Melee, Data);
	
	
}
