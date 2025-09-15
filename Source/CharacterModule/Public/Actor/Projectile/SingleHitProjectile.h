// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/BrandNewProjectileBase.h"
#include "SingleHitProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ASingleHitProjectile : public ABrandNewProjectileBase
{
	GENERATED_BODY()

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
