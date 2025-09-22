// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Character/BrandNewBaseCharacter.h"
#include "Interfaces/BrandNewEnemyInterface.h"
#include "Interfaces/PoolableActorInterface.h"
#include "BrandNewEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedDelegate, const float, NewValue);

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

	/* begin IBrandNewEnemyInterface */
	virtual float GetXPReward() const override;
	/* end IBrandNewEnemyInterface */

	/* begin IBrandNewCharacterInterface */
	virtual void OnCharacterDied_Implementation() override;
	virtual void OnCharacterHit_Implementation(const bool bIsHit) override;
	/* end IBrandNewCharacterInterface*/

	/* begin IPoolableActorInterface */
	virtual bool IsAllocatedToWorld() override;
	/* end IPoolableActorInterface */

	void ActivateEnemy(const FVector& NewLocation, const FRotator& NewRotation = FRotator::ZeroRotator);

protected:
	virtual void BeginPlay() override;

	virtual void BindGameplayTagDelegates() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

	/* 에너미의 이름으로 데이터 에셋에서 에너미의 정보를 찾을때 필요함 */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Enemy Data")
	FName EnemyName = NAME_None;
	
	UPROPERTY(EditAnywhere, Category = "Brandnew|Enemy Data")
	FScalableFloat XPReward;
	
	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Enemy Data")
	float DeathWaiteDuration = 10.f;

	UPROPERTY(EditAnywhere, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> SecondaryAttributeDataTable;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Data Asset")
	TSoftObjectPtr<UDataAsset_EnemyAbilities> EnemyAbilitiesDataAsset;


private:
	FSecondaryAttributeDataRow* FindEnemyDataRow() const;
	void ApplyEnemyAttribute() const;
	void GiveAbilitiesToEnemy();
	void BindAttributeChanged();
	void OnStrafingTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnAttributeValueChangedDelegate HealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Brandnew|Delegates")
	FOnAttributeValueChangedDelegate MaxHealthChangedDelegate;

	UPROPERTY()
	TObjectPtr<AController> CachedController;

	int32 EnemyLevel = 1;
	bool bIsActivated = false; // 현재 서버에서만 설정됨. 필요시 Replicate 해야함.
	

public:
	FORCEINLINE void SetLevel(const int32 NewLevel) { EnemyLevel = NewLevel; };
	
};
