// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class ABrandNewEnemyCharacter;
class UBoxComponent;

USTRUCT(BlueprintType)
struct FEnemySpawnPrams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABrandNewEnemyCharacter> EnemyClass = nullptr;

	/* 스폰할 에너미 수 */
	UPROPERTY(EditAnywhere)
	int32 NumOfEnemy = 1;

	UPROPERTY(EditAnywhere)
	int32 EnemyLevel = 1;
	
};

UCLASS()
class CHARACTERMODULE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(EditAnywhere, Category = "Brandnew|Spawn")
	float SpawnExtension = 500.f;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Spawn")
	TArray<FEnemySpawnPrams> EnemyInfoToSpawn;

private:
	/* Nav Mesh 내에서 유효한 위치를 구하는 함수 */
	bool TryGetRandomLocation(FVector& OutLocation, const float Radius) const;
	/* 해당 위치에 Pawn이 존재하는지 여부를 구분하는 함수 */
	bool IsLocationFree(const FVector& LocationToCheck, const float CapsuleRadius) const;
	void SpawnEnemy(const FEnemySpawnPrams& SpawnParams, const FVector& SpawnLocation) const;

	int32 NumOfSpawn = 0;
};
