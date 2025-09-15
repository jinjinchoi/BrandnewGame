// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/SingleHitProjectile.h"

#include "CharacterFunctionLibrary.h"
#include "Interfaces/BrandNewCharacterInterface.h"

void ASingleHitProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !GetOwner() || OtherActor == GetOwner()) return;

	if (OtherActor->Implements<UBrandNewCharacterInterface>())
	{
		if (UCharacterFunctionLibrary::IsTargetActorHostile(GetOwner(), OtherActor))
		{
			ApplyDamageToTarget(OtherActor);
			OnHit();
			RemoveProjectile();
		}
	}
	else
	{
		OnHit();
		RemoveProjectile();
	}
	
}
