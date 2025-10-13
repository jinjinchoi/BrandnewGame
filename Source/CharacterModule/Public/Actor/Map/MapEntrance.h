// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapEntrance.generated.h"

class UBoxComponent;

UCLASS()
class CHARACTERMODULE_API AMapEntrance : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapEntrance();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	// 해당 Entrance로 이동할 맵 클래스
	UPROPERTY(EditAnywhere, Category = "Brandnew|Level")
	TSoftObjectPtr<UWorld> LevelToTravelClass;

	// 트랜지션 맵 클래스	
	UPROPERTY(EditAnywhere, Category = "Brandnew|Level")
	TSoftObjectPtr<UWorld> TransitionLevelClass;


private:
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> OverlappingActors;

	/* 액터가 제거되었다는 것은 플레이어가 나갔다는 것이기 때문에 Set에서 제외하고 다시 체크하여 맵 이동할지 여부를 확인 */
	UFUNCTION()
	void OnActorDestroyed(AActor* DestroyedActor);

	void CleanupInvalidActors();

	// 모든 플레이어가 오버랩 되었으면 다음 맵으로 이동.
	void CheckAllPlayersOverlapped();
	


};
