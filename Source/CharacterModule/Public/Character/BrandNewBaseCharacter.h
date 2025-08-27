// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/BrandNewCharacterInterface.h"
#include "BrandNewBaseCharacter.generated.h"

class UBrandNewAbilitySystemComponent;
class ABrandNewWeapon;
class UGameplayEffect;
class UAttributeSet;

UCLASS()
class CHARACTERMODULE_API ABrandNewBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IBrandNewCharacterInterface
{
	GENERATED_BODY()

public:
	ABrandNewBaseCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* begin IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* end IAbilitySystemInterface */

	/* begin IBrandNewCharacterInterface */
	virtual void ToggleWeaponCollision_Implementation(bool bEnable) override;
	/* end IBrandNewCharacterInterface */

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

	UPROPERTY(Replicated)
	TObjectPtr<ABrandNewWeapon> CombatWeapon;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	FName CombatSocketName = FName("CombatSocket");

private:
	void ApplyPrimaryAttribute() const;
	void SetupWeapon();

public:
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
};
