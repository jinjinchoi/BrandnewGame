// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/MultiHitProjectile.h"

#include "CharacterFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Interfaces/BrandNewCharacterInterface.h"


void AMultiHitProjectile::LaunchProjectile(const FVector& SpawnLocation, const FVector& TargetLocation)
{
	Super::LaunchProjectile(SpawnLocation, TargetLocation);
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ThisClass::ApplyPeriodicDamage, DamageInterval, true);
}

void AMultiHitProjectile::RemoveProjectile()
{
	Super::RemoveProjectile();
	OverlappedActors.Empty();
}

void AMultiHitProjectile::ApplyPeriodicDamage()
{
	if (!GetOwner()) return;

	SyncOverlaps();
	
	for (TSet<AActor*>::TIterator It = OverlappedActors.CreateIterator(); It; ++It)
	{
		AActor* Target = *It;
		if (!IsValid(Target))
		{
			It.RemoveCurrent();
			continue;
		}

		NetMulticast_GenerateHitEffect(Target->GetActorLocation());
		ApplyDamageToTarget(Target);
	}
}

void AMultiHitProjectile::SyncOverlaps()
{
	TArray<AActor*> CurrentOverlaps;
	SphereCollision->GetOverlappingActors(CurrentOverlaps, AActor::StaticClass());
	
	OverlappedActors.Empty();

	for (AActor* Actor : CurrentOverlaps)
	{
		if (!Actor) continue;

		if (Actor->Implements<UBrandNewCharacterInterface>() &&
			UCharacterFunctionLibrary::IsTargetActorHostile(Actor, GetOwner()))
		{
			OverlappedActors.Add(Actor);
		}
	}
}

void AMultiHitProjectile::NetMulticast_GenerateHitEffect_Implementation(const FVector_NetQuantize10& HitLocation)
{
	OnHit(HitLocation);
}
