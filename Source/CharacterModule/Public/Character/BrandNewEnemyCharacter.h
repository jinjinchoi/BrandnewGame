// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Character/BrandNewBaseCharacter.h"
#include "BrandNewEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedDelegate, const float, NewValue);

class UWidgetComponent;
class UDataAsset_EnemyAbilities;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewEnemyCharacter : public ABrandNewBaseCharacter
{
	GENERATED_BODY()

public:
	ABrandNewEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	/* 에너미의 이름으로 데이터 에셋에서 에너미의 정보를 찾을때 필요함 */
	UPROPERTY(EditAnywhere, Category = "BrandNew|Enemy Data")
	FName EnemyName = NAME_None;
	
	UPROPERTY(EditAnywhere, Category = "BrandNew|Enemy Data")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, Category = "BrandNew|DataTable")
	TObjectPtr<UDataTable> SecondaryAttributeDataTable;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSoftObjectPtr<UDataAsset_EnemyAbilities> EnemyAbilitiesDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;

private:
	FSecondaryAttributeDataRow* FindEnemyDataRow() const;
	void ApplyEnemyAttribute() const;
	void GiveAbilitiesToEnemy();
	void BindAttributeChanged();

	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnAttributeValueChangedDelegate HealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "BrandNew|Delegates")
	FOnAttributeValueChangedDelegate MaxHealthChangedDelegate;
	
};
