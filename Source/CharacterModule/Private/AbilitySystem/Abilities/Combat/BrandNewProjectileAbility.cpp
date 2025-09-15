// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/BrandNewProjectileAbility.h"

#include "Actor/Projectile/BrandNewProjectileBase.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "Manager/Pooling/BrandNewObjectPoolManager.h"

void UBrandNewProjectileAbility::SpawnProjectile(const FVector& SpawnLocation, const FVector& TargetLocation, AActor* TargetActor)
{
	check(ProjectileClass);
	
    if (!HasAuthority(&CurrentActivationInfo)) return;

	UBrandNewObjectPoolManager* ObjectPoolManager = UBrandNewFunctionLibrary::GetObjectPoolManager(GetAvatarActorFromActorInfo());
	if (!ObjectPoolManager) return;

	AActor* PooledObject  = ObjectPoolManager->GetPooledObject(ProjectileClass);
	if (!PooledObject) return;

	ABrandNewProjectileBase* Projectile = CastChecked<ABrandNewProjectileBase>(PooledObject);
	Projectile->InitProjectile(GetAvatarActorFromActorInfo(), MakeNonTargetEffectParams());

	if (bIsHomingProjectile)
	{
		Projectile->SetHomingTarget(TargetActor, bRotationFollowsVelocity);
	}
	
	Projectile->LaunchProjectile(SpawnLocation, TargetLocation);
	
}
