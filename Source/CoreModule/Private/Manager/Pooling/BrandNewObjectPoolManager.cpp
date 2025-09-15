// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Pooling/BrandNewObjectPoolManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PoolableActorInterface.h"

void UBrandNewObjectPoolManager::InitPoolManager()
{
	CreatePool(ProjectileActorClasses);
	CreatePool(CharacterClasses);
}

AActor* UBrandNewObjectPoolManager::GetPooledObject(const TSubclassOf<AActor>& ActorClass)
{
	if (ActorClass == nullptr) return nullptr;

	// Pool에서 Actor 배열을 가져옴
	TArray<AActor*>* PoolActors = ObjectPools.Find(ActorClass);
	if (PoolActors == nullptr) return nullptr;
	
	for (AActor* PoolActor : *PoolActors)
	{
		IPoolableActorInterface* PoolableActorInterface = Cast<IPoolableActorInterface>(PoolActor);
		if (!PoolableActorInterface) continue;
		
		if (IsValid(PoolActor) && !PoolableActorInterface->IsAllocatedToWorld())
		{
			if (const ACharacter* Character = Cast<ACharacter>(PoolActor))
			{
				Character->GetCharacterMovement()->GravityScale = 1.f;
			}
			
			return PoolActor;
		}
	}

	if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator))
	{
		ObjectPools[ActorClass].Add(NewActor);
		
		return NewActor;
	}

	return nullptr;
}

void UBrandNewObjectPoolManager::ReturnObject(AActor* ActorToReturn)
{
	if (!IsValid(ActorToReturn)) return;
	
	ActorToReturn->SetActorHiddenInGame(true);
	ActorToReturn->SetActorEnableCollision(false);
	ActorToReturn->SetActorTickEnabled(false);

	if (const ACharacter* Character = Cast<ACharacter>(ActorToReturn))
	{
		Character->GetCharacterMovement()->GravityScale = 0.f;
		UAnimInstance* OwningAnimInstance = Character->GetMesh()->GetAnimInstance();

		if (OwningAnimInstance && OwningAnimInstance->IsAnyMontagePlaying())
		{
			OwningAnimInstance->StopAllMontages(0.1f);
		}
	}
}

void UBrandNewObjectPoolManager::CreatePool(const TArray<TSubclassOf<AActor>>& InObjectClasses)
{
	for (const TSubclassOf<AActor>& ObjClass : InObjectClasses)
	{
		TArray<AActor*>& Pool = ObjectPools.FindOrAdd(ObjClass);
		for (int32 i = 0; i < PoolSizePerType; ++i)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			if (AActor* PoolActor = GetWorld()->SpawnActor<AActor>(ObjClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
			{
				PoolActor->SetActorHiddenInGame(true);
				PoolActor->SetActorEnableCollision(false);
				PoolActor->SetActorTickEnabled(false);

				if (const ACharacter* Character = Cast<ACharacter>(PoolActor))
				{
					Character->GetCharacterMovement()->StopMovementImmediately();
					Character->GetCharacterMovement()->GravityScale = 0.f;
				}
				
				Pool.Add(PoolActor);
			}
		}
	}
}
