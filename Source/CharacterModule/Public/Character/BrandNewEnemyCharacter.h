// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Character/BrandNewBaseCharacter.h"
#include "BrandNewEnemyCharacter.generated.h"

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
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Enemy Data")
	FName EnemyName = NAME_None;
	
	UPROPERTY(EditAnywhere, Category = "BrandNew|Enemy Data")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, Category = "BrandNew|DataTable")
	TObjectPtr<UDataTable> SecondaryAttributeDataTable;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

private:
	FSecondaryAttributeDataRow* FindEnemyDataRow() const;
	void ApplyEnemyAttribute() const;
	
};
