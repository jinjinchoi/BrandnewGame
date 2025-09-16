// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Projectile/BrandNewProjectileBase.h"
#include "MultiHitProjectile.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API AMultiHitProjectile : public ABrandNewProjectileBase
{
	GENERATED_BODY()

protected:
	/* begin AVNProjectileBase */
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void LaunchProjectile(const FVector& SpawnLocation, const FVector& TargetLocation) override;
	virtual void RemoveProjectile() override;
	/* end AVNProjectileBase */

	void ApplyPeriodicDamage();
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties | Combat")
	float DamageInterval = 0.5;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_GenerateHitEffect(const FVector_NetQuantize10& HitLocation);
	
	FTimerHandle DamageTimerHandle;
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;
	
};
