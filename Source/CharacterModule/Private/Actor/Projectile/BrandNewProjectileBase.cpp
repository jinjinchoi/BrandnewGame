// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/BrandNewProjectileBase.h"

#include "CharacterFunctionLibrary.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/BrandNewCharacterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DebugHelper.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "Manager/Pooling/BrandNewObjectPoolManager.h"
#include "Net/UnrealNetwork.h"

ABrandNewProjectileBase::ABrandNewProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
	SetRootComponent(SphereCollision);
	SphereCollision->SetCollisionObjectType(ECC_Projectile);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Effect Component"));
	NiagaraComponent->SetupAttachment(GetRootComponent());
	NiagaraComponent->bAutoActivate = false;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement Component"));
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	
}

void ABrandNewProjectileBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, IsActivated);
	
}


void ABrandNewProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	
	if (!LoopingSoundComponent && LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
		LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
	}
	
	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereEndOverlap);

}

void ABrandNewProjectileBase::InitProjectile(AActor* ProjectileOwner, const FDamageEffectParams& InDamageEffectParams)
{
	check(ProjectileOwner);
	SetOwner(ProjectileOwner);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent->InitialSpeed =InitSpeed;
	ProjectileMovementComponent->MaxSpeed = InitSpeed;
	
}

void ABrandNewProjectileBase::SetHomingTarget(AActor* Target, const bool bRotationFollowsVelocity)
{
	if (Target == nullptr) return;

	HomingTargetActor = Target;
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->HomingTargetComponent = HomingTargetActor->GetRootComponent();
	ProjectileMovementComponent->bRotationFollowsVelocity = bRotationFollowsVelocity;
	ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	if (IBrandNewCharacterInterface* CombatInterface = Cast<IBrandNewCharacterInterface>(HomingTargetActor))
	{
		CombatInterface->GetOnCharacterDiedDelegate().AddUObject(this, &ThisClass::OnTargetDestroyed);
	}
}

void ABrandNewProjectileBase::LaunchProjectile(const FVector& SpawnLocation, const FVector& TargetLocation)
{
	GetWorldTimerManager().SetTimer(ProjectileLifeSpanTimer, this, &ThisClass::RemoveProjectile, MaxFlightTime, false);
	
	FVector FlatTargetLocation = TargetLocation;
	FlatTargetLocation.Z = SpawnLocation.Z;
	const FVector Direction = (FlatTargetLocation - SpawnLocation).GetSafeNormal();

	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Play();
	}
	
	SetActorLocation(SpawnLocation);
	SetActorRotation(Direction.Rotation());
	SetActorHiddenInGame(false);
	IsActivated = true;
	SetActorEnableCollision(true);
	
	ProjectileMovementComponent->Velocity = InitSpeed * Direction;
	ProjectileMovementComponent->UpdateComponentVelocity();
}

bool ABrandNewProjectileBase::IsAllocatedToWorld()
{
	return IsActivated;
}


void ABrandNewProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABrandNewProjectileBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void ABrandNewProjectileBase::OnHit(const FVector& LocationOverride) const
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	}

	if (ImpactEffect)
	{
		if (LocationOverride != FVector::ZeroVector)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, LocationOverride, GetActorRotation());
		}
		else
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), GetActorRotation());
		}
	}
}

void ABrandNewProjectileBase::ApplyDamageToTarget(AActor* DamagedActor)
{
	if (!DamagedActor) return;
	if (!DamagedActor->Implements<UBrandNewCharacterInterface>() || IBrandNewCharacterInterface::Execute_IsDead(DamagedActor)) return;
	if (!UCharacterFunctionLibrary::IsTargetActorHostile(this, DamagedActor)) return;

	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(DamagedActor);
	DamageEffectParams.HitDirection = UCharacterFunctionLibrary::ComputeHitReactDirection(this, DamagedActor);
	const FVector ToTarget = (DamagedActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	DamageEffectParams.KnockbackImpulse = ToTarget * KnockbackForce;

	UCharacterFunctionLibrary::ApplyDamageEffect(DamageEffectParams);
	
}

void ABrandNewProjectileBase::RemoveProjectile()
{

	if (DestructionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestructionEffect, GetActorLocation(), GetActorRotation());
	}

	if (DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation(), GetActorRotation());
	}

	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}

	if (HasAuthority())
	{
		if (UBrandNewObjectPoolManager* PoolManager = UBrandNewFunctionLibrary::GetObjectPoolManager(this))
		{
			ProjectileMovementComponent->StopMovementImmediately();
			IsActivated = false;
			PoolManager->ReturnObject(this);
		}
	}
	
}

void ABrandNewProjectileBase::OnTargetDestroyed()
{
	if (IBrandNewCharacterInterface* CombatInterface = Cast<IBrandNewCharacterInterface>(HomingTargetActor))
	{
		CombatInterface->GetOnCharacterDiedDelegate().RemoveAll(this);
		HomingTargetActor = nullptr;
	}
	
	// bAutoRetarget이 설정되어 있으면 오너로부터 가장 가까운 적을 찾아 호밍 타겟을 재설정.
	if (bAutoRetarget && ProjectileMovementComponent->bIsHomingProjectile && GetOwner())
	{
		TArray<AActor*> OverlappedActors;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		IgnoreActors.Add(this);
		
		UCharacterFunctionLibrary::GetHostileActorsWithinRadius(
			this, GetOwner(), OverlappedActors, IgnoreActors, RetargetRadius, GetActorLocation()
		);

		if (OverlappedActors.Num() > 0)
		{
			HomingTargetActor = UCharacterFunctionLibrary::GetClosestActor(OverlappedActors, GetActorLocation());
			ProjectileMovementComponent->HomingTargetComponent = HomingTargetActor->GetRootComponent();

			return;
			
		}
	}
	
	// 오토 타겟이 아니거나 그 외 유효하지 않는 상황에서는 발사체 제거
	RemoveProjectile();
}

void ABrandNewProjectileBase::OnRep_IsActivated()
{
	SetActorHiddenInGame(!IsActivated);
	if (NiagaraComponent && IsActivated)
	{
		NiagaraComponent->Activate();
	}
}
