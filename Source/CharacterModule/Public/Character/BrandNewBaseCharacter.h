// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/BrandNewCharacterInterface.h"
#include "BrandNewBaseCharacter.generated.h"

class UBrandNewAttributeSet;
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
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const;

	UPROPERTY()
	TObjectPtr<UBrandNewAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBrandNewAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	TSubclassOf<ABrandNewWeapon> CombatWeaponClass;

	UPROPERTY(Replicated)
	TObjectPtr<ABrandNewWeapon> CombatWeapon;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	FName CombatSocketName = FName("CombatSocket");

private:
	void SetupWeapon();

public:
	FORCEINLINE UBrandNewAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
};
