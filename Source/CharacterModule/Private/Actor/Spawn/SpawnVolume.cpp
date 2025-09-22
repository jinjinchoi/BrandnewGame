// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Spawn/SpawnVolume.h"

#include "NavigationSystem.h"
#include "Character/BrandNewEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "Manager/Pooling/BrandNewObjectPoolManager.h"


ASpawnVolume::ASpawnVolume()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(BoxCollision);

	BoxCollision->SetBoxExtent(FVector(500.f, 500.f, 200.f));

}


void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	for (const FEnemySpawnPrams& SpawnPrams : EnemyInfoToSpawn)
	{
		NumOfSpawn += SpawnPrams.NumOfEnemy; // 스폰할 에너미의 수를 계산
	}
	
	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	
	
}

void ASpawnVolume::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	BoxCollision->OnComponentBeginOverlap.Clear();

	constexpr int32 MaxAttemptsPerEnemy = 50.f;

	for (const FEnemySpawnPrams& SpawnParams : EnemyInfoToSpawn)
	{
		if (!SpawnParams.EnemyClass) continue;;

		int32 SpawnedCount = 0;
		int32 AttemptsCount = 0;

		while (SpawnedCount < SpawnParams.NumOfEnemy && AttemptsCount < MaxAttemptsPerEnemy)
		{
			++AttemptsCount;

			FVector SpawnLocation;
			if (!TryGetRandomLocation(SpawnLocation, SpawnExtension))
			{
				continue; // NavMesh 랜덤 뽑기 실패 → 다시 시도
			}

			DrawDebugPoint(GetWorld(), SpawnLocation, 6.f, FColor::Blue, false, 5.f); // 소환 시도한 위치 Debug

			if (!IsLocationFree(SpawnLocation, 100.f))
			{
				continue; // 겹침 → 다시 시도
			}

			DrawDebugSphere(GetWorld(), SpawnLocation, 100.f, 12, FColor::Red, false, 5.f); // 실제 소환 위치 Debug
			SpawnEnemy(SpawnParams, SpawnLocation);
			++SpawnedCount;
		}
		
	}
	
}

bool ASpawnVolume::TryGetRandomLocation(FVector& OutLocation, const float Radius) const
{
	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLocation;
		if (NavSys->GetRandomPointInNavigableRadius(GetActorLocation(), Radius, NavLocation))
		{
			OutLocation = NavLocation.Location;
			return true;
		}
	}
	return false;
}

bool ASpawnVolume::IsLocationFree(const FVector& LocationToCheck, const float CapsuleRadius) const
{
	const UWorld* World = GetWorld();
	if (!World) return false;

	// Pawn 타입(다른 적)만 검사하도록 설정
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FOverlapResult> Overlaps;
	const FCollisionShape CollisionShape  = FCollisionShape::MakeSphere(CapsuleRadius);

	const bool bHasOverlap = World->OverlapMultiByObjectType(
		Overlaps,
		LocationToCheck,
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape
	);

	// Pawn과 겹치면 false, 겹치지 않으면 true
	return !bHasOverlap;
	
}

void ASpawnVolume::SpawnEnemy(const FEnemySpawnPrams& SpawnParams, const FVector& SpawnLocation) const
{
	UBrandNewObjectPoolManager* ObjectPoolManager = UBrandNewFunctionLibrary::GetObjectPoolManager(this);
	if (!ObjectPoolManager) return;

	AActor* PooledActor = ObjectPoolManager->GetPooledObject(SpawnParams.EnemyClass);
	if (!PooledActor) return;

	ABrandNewEnemyCharacter* Enemy = CastChecked<ABrandNewEnemyCharacter>(PooledActor);
	Enemy->SpawnDefaultController();
	Enemy->SetLevel(SpawnParams.EnemyLevel);
	Enemy->ActivateEnemy(SpawnLocation, GetActorRotation());
}


