// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BrandNewBaseCharacter.generated.h"

class UBrandNewAbilitySystemComponent;
class ABrandNewWeapon;
class UGameplayEffect;
class UAttributeSet;

UCLASS()
class CHARACTERMODULE_API ABrandNewBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABrandNewBaseCharacter();

	/* begin IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* end IAbilitySystemInterface */

protected:
	/* begin Actor Class */
	virtual void BeginPlay() override;
	/* end Actor Class */
	
	virtual void InitAbilityActorInfo();

	UPROPERTY()
	TObjectPtr<UBrandNewAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "BrandNew|DataTable")
	TObjectPtr<UDataTable> AttributeDataTable;

	UPROPERTY(EditAnywhere, Category = "BrandNew|DataTable")
	FName AttributeTableKeyName;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSubclassOf<UGameplayEffect> PrimaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	TSubclassOf<ABrandNewWeapon> CombatWeaponClass;

	UPROPERTY()
	TObjectPtr<ABrandNewWeapon> CombatWeapon;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	FName CombatSocketName = FName("CombatSocket");

private:
	void ApplyPrimaryAttribute() const;
	void SetupWeapon();

public:
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
};
