// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/MultiHitProjectile.h"

#include "CharacterFunctionLibrary.h"
#include "Interfaces/BrandNewCharacterInterface.h"


void AMultiHitProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UBrandNewCharacterInterface>() && UCharacterFunctionLibrary::IsTargetActorHostile(OtherActor, GetOwner()))
	{
		OverlappedActors.Add(OtherActor); 
	}
}

void AMultiHitProjectile::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActors.Remove(OtherActor);
}

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
	
	for (TSet<TWeakObjectPtr<AActor>>::TIterator It = OverlappedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
			continue;
		}
		
		AActor* Target = It->Get();
		NetMulticast_GenerateHitEffect(Target->GetActorLocation());
		ApplyDamageToTarget(Target);
	}
}

void AMultiHitProjectile::NetMulticast_GenerateHitEffect_Implementation(const FVector_NetQuantize10& HitLocation)
{
	OnHit(HitLocation);
}
