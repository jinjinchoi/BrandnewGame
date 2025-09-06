// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Interfaces/BrandNewCharacterInterface.h"
#include "BrandNewBaseCharacter.generated.h"

class UBrandNewAttributeSet;
class UBrandNewAbilitySystemComponent;
class ABrandNewWeapon;
class UGameplayEffect;
class UAttributeSet;
class UMotionWarpingComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IBrandNewCharacterInterface
{
	GENERATED_BODY()

public:
	ABrandNewBaseCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/* begin Character Interface */
	virtual void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) override;
	/* end Character Interface */

	/* begin IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* end IAbilitySystemInterface */

	/* begin IBrandNewCharacterInterface */
	virtual void ToggleWeaponCollision_Implementation(bool bEnable) override;
	virtual void OnCharacterHit_Implementation(const bool bIsHit) override;
	virtual void OnCharacterDied_Implementation() override;
	virtual bool IsHitReacting() const override;
	virtual UMotionWarpingComponent* GetMotionWarpingComponent_Implementation() override;
	/* end IBrandNewCharacterInterface */



protected:
	/* begin Actor Class */
	virtual void BeginPlay() override;
	/* end Actor Class */
	
	virtual void InitAbilityActorInfo();
	virtual void BindGameplayTagDelegates();
	
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	
	UPROPERTY()
	TObjectPtr<UBrandNewAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBrandNewAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	TSubclassOf<ABrandNewWeapon> CombatWeaponClass;

	UPROPERTY(Replicated)
	TObjectPtr<ABrandNewWeapon> CombatWeapon;

	/* 무기를 부착할 소켓 이름 */
	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	FName CombatSocketName = FName("CombatSocket");

	UPROPERTY(EditAnywhere, Category = "BrandNew|Combat")
	bool bCanLaunch = true;
	
	bool bIsHitReacting = false;
	bool bIsDead = false;

private:
	void SetupWeapon();
	void OnStrafingTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:
	FORCEINLINE UBrandNewAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
};
