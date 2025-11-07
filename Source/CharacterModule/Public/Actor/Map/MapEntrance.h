// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapEntrance.generated.h"

class UWidgetComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PortalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;
	
	// 해당 Entrance로 이동할 맵 클래스
	UPROPERTY(EditAnywhere, Category = "Brandnew|Level")
	TSoftObjectPtr<UWorld> LevelToTravelClass;

	// 트랜지션 맵 클래스	
	UPROPERTY(EditAnywhere, Category = "Brandnew|Level")
	TSoftObjectPtr<UWorld> TransitionLevelClass;

	/**
	 * InteractionWidgetComponent는 맵 이름을 표시할 것인데 블루프린트에서 설정함.
	 * 현재 NPC이름과 Map Entrance의 이동할 맵 이름이 이런 식으로 설정 중.
	 * 이러한 Interaction Widget이 한번 더 늘어나면 블루프린트가 아닌 C++에서 인터페이스 이용해서 설정하도록 수정할 예정.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brandnew|Level")
	FText MapNameToTravel;

private:
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> OverlappingActors;

	UPROPERTY()
	TObjectPtr<UUserWidget> EntryStatusWidget;

	/* 액터가 제거되었다는 것은 플레이어가 나갔다는 것이기 때문에 Set에서 제외하고 다시 체크하여 맵 이동할지 여부를 확인 */
	UFUNCTION()
	void OnPlayerJoined(const APlayerState* NewPlayerState);

	UFUNCTION()
	void OnPlayerExited(const APlayerState* ExitedPlayerState);

	void CleanupInvalidActors();

	// 모든 플레이어가 오버랩 되었으면 다음 맵으로 이동.
	void CheckAllPlayersOverlapped();

	/* 오버랩 된 플레이어의 수를 보여주는 위젯을 생성하거나 유저 수 업데이트 하는 함수 */
	void CreateOrUpdateEntryStatusWidget() const;

};
