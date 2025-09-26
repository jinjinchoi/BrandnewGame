// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/BrandNewCharacterInterface.h"
#include "BrandNewBaseCharacter.generated.h"

class UBoxComponent;
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
	virtual void ToggleWeaponCollision_Implementation(const bool bEnable) override;
	virtual void ToggleCharacterCombatCollision_Implementation(const bool bEnable, const ECombatCollisionPosition CollisionPosition) override;
	virtual void OnCharacterHit_Implementation(const bool bIsHit) override;
	virtual void OnCharacterDied_Implementation() override;
	virtual UMotionWarpingComponent* GetMotionWarpingComponent_Implementation() const override;
	virtual FVector GetProjectileSpawnLocation_Implementation(const FName& SocketName) const override;
	virtual bool IsDead_Implementation() const override;
	virtual bool IsHitReacting() const override;
	virtual FOnCharacterDied& GetOnCharacterDiedDelegate() override;
	/* end IBrandNewCharacterInterface */

protected:
	/* begin Actor Class */
	virtual void BeginPlay() override;
	/* end Actor Class */
	
	virtual void InitAbilityActorInfo();
	virtual void BindGameplayTagDelegates();
	
	void ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const;

#if WITH_EDITOR
	/* Begin UObject */
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	/* End UObject */
#endif

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Warping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	
	UPROPERTY()
	TObjectPtr<UBrandNewAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UBrandNewAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = "Brandnew|Combat")
	TSubclassOf<ABrandNewWeapon> CombatWeaponClass;

	UPROPERTY(Replicated)
	TObjectPtr<ABrandNewWeapon> CombatWeapon;

	/* 무기를 부착할 소켓 이름 */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Combat")
	FName CombatSocketName = FName("CombatSocket");

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> LeftHandCollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Combat")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(EditDefaultsOnly, Category="Brandnew|Combat")
	FName RightHandCollisionBoxAttachBoneName;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Combat")
	bool bCanLaunch = true;
	
	bool bIsHitReacting = false;
	bool bIsDead = false;

private:
	void SetupWeapon();

	FOnCharacterDied OnCharacterDiedDelegate;
	
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;

public:
	FORCEINLINE UBrandNewAttributeSet* GetAttributeSet() const { return AttributeSet; }

};
