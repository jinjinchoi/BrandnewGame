// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Combat/BandNewDamageAbilityBase.h"
#include "BrandNewProjectileAbility.generated.h"

class ABrandNewProjectileBase;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewProjectileAbility : public UBandNewDamageAbilityBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile(const FVector& SpawnLocation, const FVector& TargetLocation, AActor* TargetActor = nullptr);
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Proejctile", meta = (MustImplement = "PoolableActorInterface"))
	TSubclassOf<ABrandNewProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrandNew|Proejctile")
	FName ProjectileSpawnSocketName = TEXT("ProjectileSocket");

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Proejctile")
	bool bIsHomingProjectile;

	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Proejctile", meta = (EditCondition = "bIsHomingProjectile"))
	bool bRotationFollowsVelocity = true;
	
};
