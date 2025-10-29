// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/Pooling/BrandNewObjectPoolManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Actor/PoolableActorInterface.h"

void UBrandNewObjectPoolManager::InitPoolManager()
{
	
	TargetObjectClasses = GetCurrentLevelSpawnActorClasses();
	if (TargetObjectClasses.IsEmpty()) return;
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ThisClass::TimerTick, 0.1f, true);
	
}

void UBrandNewObjectPoolManager::TimerTick()
{
	if (bInitComplete)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		TargetObjectClasses.Empty();
		return;
	}
	
	SpawnStep();
}

void UBrandNewObjectPoolManager::SpawnStep()
{
	if (TargetObjectClasses.IsEmpty()) return;

	// 배열 유효하지 않으면 풀 초기화 완료된 것으로 설정
	if (!TargetObjectClasses.IsValidIndex(CurrentClassIndex))
	{
		bInitComplete = true;
		return;
	}
	
	const TSubclassOf<AActor> ObjectClass = TargetObjectClasses[CurrentClassIndex];
	TArray<AActor*>& Pool = ObjectPools.FindOrAdd(ObjectClass);

	// 현재 프레임에 생성한 액터 수
	int32 SpawnedThisFrame = 0;

	// 한 프레임에 최대 스폰 횟수만큼 스폰하거나 풀에 할당량만큼 스폰되면 루프 종료
	while (SpawnedThisFrame < SpawnLimitPerFrame && CurrentSpawnCount < PoolSizePerType)
	{
		// 스폰 로직
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
		if (AActor* PoolActor = GetWorld()->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams))
		{
			PoolActor->SetActorHiddenInGame(true);
			PoolActor->SetActorEnableCollision(false);
			PoolActor->SetActorTickEnabled(false);

			if (const ACharacter* Character = Cast<ACharacter>(PoolActor))
			{
				Character->GetCharacterMovement()->StopMovementImmediately();
				Character->GetCharacterMovement()->GravityScale = 0.f;
				Character->GetMesh()->bPauseAnims = true;
			}
				
			Pool.Add(PoolActor);
		}

		// 현 프레임에 스폰됫 횟수와 전체 스폰 횟수 증가
		++SpawnedThisFrame;
		++CurrentSpawnCount;
		
	}

	// 루프 종료 후 전체 스폰 횟수가 스폰 해야할 횟수를 넘으면 다음 배열로 이동 
	if (CurrentSpawnCount >= PoolSizePerType)
	{
		CurrentSpawnCount = 0;
		++CurrentClassIndex;
	}
	
	
}

TArray<TSubclassOf<AActor>> UBrandNewObjectPoolManager::GetCurrentLevelSpawnActorClasses() const
{
	for (const FLevelPoolData& SpawnParam : SpawnDataPrams)
	{
		if (IsSameLevel(SpawnParam.LevelWorld))
		{
			TArray<TSubclassOf<AActor>> ObjectClassesToSpawn = SpawnParam.ObjectClasses;
			ObjectClassesToSpawn.Append(SharedObjectClasses);
			
			return ObjectClassesToSpawn;
		}
	}

	return TArray<TSubclassOf<AActor>>();
	
}

bool UBrandNewObjectPoolManager::IsSameLevel(const TSoftObjectPtr<UWorld> TargetWorld) const
{
	const UWorld* World = GetWorld();
	if (!World) return false;

	FString CurrentMapName = World->GetOutermost()->GetName();
	FString TargetMapName = TargetWorld.ToSoftObjectPath().GetAssetName(); 
	
#if WITH_EDITOR
	// PIE 접두사 제거
	CurrentMapName = UWorld::RemovePIEPrefix(CurrentMapName);
	TargetMapName = UWorld::RemovePIEPrefix(CurrentMapName);
#endif


	return CurrentMapName == TargetMapName;
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
			return PoolActor;
		}
	}

	// Pool에 Actor가 없으면 새로 만들어서 풀에 등록하고 반환
	if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator))
	{
		NewActor->SetActorHiddenInGame(true);
		NewActor->SetActorEnableCollision(false);
		NewActor->SetActorTickEnabled(false);
		
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
