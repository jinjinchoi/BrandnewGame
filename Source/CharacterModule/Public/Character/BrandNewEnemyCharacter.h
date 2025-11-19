// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Character/BrandNewBaseCharacter.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "Interfaces/Character/BrandNewEnemyInterface.h"
#include "Interfaces/Actor/PoolableActorInterface.h"
#include "BrandNewEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedDelegate, const float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayTagChanged, const FGameplayTag&, NewTag, bool, bTagAdded);

class UWidgetComponent;
class UDataAsset_EnemyAbilities;

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewEnemyCharacter : public ABrandNewBaseCharacter, public IBrandNewEnemyInterface, public IPoolableActorInterface
{
	GENERATED_BODY()

public:
	ABrandNewEnemyCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* begin IBrandNewEnemyInterface */
	virtual float GetXPReward() const override;
	virtual FName GetEnemyName() const override;
	virtual float GetInitialMaxWalkSpeed_Implementation() const override;
	/* end IBrandNewEnemyInterface */

	/* begin IBrandNewCharacterInterface */
	virtual void OnCharacterDied_Implementation() override;
	virtual void OnCharacterHit_Implementation(const bool bIsHit) override;
	/* end IBrandNewCharacterInterface*/

	/* begin IPoolableActorInterface */
	virtual bool IsAllocatedToWorld() override;
	/* end IPoolableActorInterface */

	void SetEnemyTransform(const FVector& NewLocation, const FRotator& NewRotation = FRotator::ZeroRotator);
	void SetEnemyAttribute(const int32 NewLevel);
	void ActivateEnemy();

protected:
	virtual void BeginPlay() override;
	
	virtual void BindGameplayTagChanged() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

	/* 에너미의 이름으로 데이터 에셋에서 에너미의 정보를 찾을때 및 퀘스트 카운트 올릴 때 사용 */
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|EnemyData")
	FName EnemyName = NAME_None;

	/* 화면에 보여줄 에너미의 이름 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|EnemyData")
	FText EnemyDisplayName;
	
	UPROPERTY(ReplicatedUsing = "OnRep_EnemyLevel", VisibleAnywhere, BlueprintReadOnly)
	int32 EnemyLevel = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|EnemyData")
	FScalableFloat XPReward;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|EnemyData")
	float DeathWaiteDuration = 6.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|EnemyData")
	float InitialMaxWalkSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> SecondaryAttributeDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Data Asset")
	TSoftObjectPtr<UDataAsset_EnemyAbilities> EnemyAbilitiesDataAsset;

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void StartAppearEffect();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyLevelChanged();
	
	UFUNCTION()
	void OnRep_EnemyLevel();

private:
	FSecondaryAttributeDataRow* FindEnemyDataRow() const;
	void ApplyEnemyAttribute() const;
	void GiveAbilitiesToEnemy();
	void BindAttributeChanged();
	void OnStrafingTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	void OnSuperArmorTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnAttributeValueChangedDelegate HealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnAttributeValueChangedDelegate MaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnGameplayTagChanged GameplayTagChangedDelegate;
	
	/* Pool에서 나와 활성화 되어 있는지 확인하는 변수 */
	bool bIsActivated = false; // 현재 서버에서만 설정됨. 필요시 Replicate 해야함.

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EnableCapsuleCollision();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ActivateDisappearEffect();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ActivateAppearEffect();
	
};
