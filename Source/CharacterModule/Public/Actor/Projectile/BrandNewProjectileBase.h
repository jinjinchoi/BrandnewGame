// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Combat/BandNewDamageAbilityBase.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PoolableActorInterface.h"
#include "BrandNewProjectileBase.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USphereComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewProjectileBase : public AActor, public IPoolableActorInterface
{
	GENERATED_BODY()
	
public:	
	ABrandNewProjectileBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* 아래 세개의 함수는 서버에서만 실행됨. */
	virtual void InitProjectile(AActor* ProjectileOwner, const FDamageEffectParams& InDamageEffectParams);
	virtual void SetHomingTarget(AActor* Target, const bool bRotationFollowsVelocity);
	virtual void LaunchProjectile(const FVector& SpawnLocation, const FVector& TargetLocation);

	/* begin IPoolableActorInterface */
	virtual bool IsAllocatedToWorld() override;
	/* end  IPoolableActorInterface*/

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void OnHit(const FVector& LocationOverride = FVector::ZeroVector) const;
	void ApplyDamageToTarget(AActor* DamagedActor);
	virtual void RemoveProjectile();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|SFX")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|SFX")
	TObjectPtr<USoundBase> ImpactSound;

	/* 타겟과 Overlap 될 때 사용할 Effect */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|SFX")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	/* Projectile 사라질 때 사용할 Effect */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|SFX")
	TObjectPtr<UNiagaraSystem> DestructionEffect;

	/* Projectile 사라질 때 사용할 Sound */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|SFX")
	TObjectPtr<USoundBase> DestructionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Proejctile")		
	float MaxFlightTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Proejctile")		
	float InitSpeed = 250.f;

	/* 발사할때 각도 없이 Owner와 동일한 일직선으로 발사할지 여부 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Projectile")
	bool bIsFlatLaunch = true;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Homing")
	float HomingAccelerationMagnitude = 5000.f;
	
	/* 호밍 중 타겟이 사라지면 자동으로 새로운 타겟 설정할지 설정하는 불리언 변수 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Homing", meta=(EditConditionToggle))
	bool bAutoRetarget = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Homing", meta = (EditCondition = "bAutoRetarget"))
	float RetargetRadius = 3000.f;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Knockback")
	float KnockbackForce = 200.f;
	
private:
	void OnTargetDestroyed();
	
	UPROPERTY()
	TObjectPtr<AActor> HomingTargetActor;
	
	FTimerHandle ProjectileLifeSpanTimer;
	/* Target ASC와 Hit 방향을 직접 설정해야함.*/
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(ReplicatedUsing = OnRep_IsActivated)
	bool IsActivated = false;

	UFUNCTION()
	void OnRep_IsActivated();
	
};
