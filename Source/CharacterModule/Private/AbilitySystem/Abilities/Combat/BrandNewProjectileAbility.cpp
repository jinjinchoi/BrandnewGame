// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Combat/BrandNewProjectileAbility.h"

#include "CharacterFunctionLibrary.h"
#include "Actor/Projectile/BrandNewProjectileBase.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
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
	
	const bool IsLockedOn = UCharacterFunctionLibrary::DoseActorHasTag(GetOwningActorFromActorInfo(), BrandNewGamePlayTag::Status_Player_LockOn);
	// 락온 중이면 락온 대상을 타겟 액터로 설정
	if (IsLockedOn)
	{
		if (AActor* CombatTarget = IBrandNewPlayerInterface::Execute_GetCombatTargetActor(GetOwningActorFromActorInfo()))
		{
			TargetActor = CombatTarget;
		}
	}

	if (bIsHomingProjectile && TargetActor)
	{
		Projectile->SetHomingTarget(TargetActor, bRotationFollowsVelocity);
	}

	const FVector TargetPos = IsLockedOn && TargetActor ? TargetActor->GetActorLocation() : TargetLocation;
	Projectile->LaunchProjectile(SpawnLocation, TargetPos);
	
}
