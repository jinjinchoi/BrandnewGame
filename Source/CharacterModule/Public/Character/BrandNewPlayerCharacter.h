// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Interfaces/BrandNewPlayerInterface.h"
#include "BrandNewPlayerCharacter.generated.h"

class UDataAsset_DefaultPlayerAbilities;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerCharacter : public ABrandNewBaseCharacter, public IBrandNewPlayerInterface
{
	GENERATED_BODY()

public:
	ABrandNewPlayerCharacter();
	/* begin Actor Interface */
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/* end Actor Interface */

	/* begin Player Interface */
	virtual ECombatWeaponType GetCurrentEquippedWeaponType() const override;
	virtual void AddYawRotation(const float DeltaYaw) override;
	virtual AActor*GetCombatWeaponActor_Implementation() const override;
	virtual void OnWeaponEquipped_Implementation() override;
	virtual void OnWeaponUnequipped_Implementation() override;
	/* end Player Interface */

	/** 캐릭터의 무브먼트 모드를 변경하는 함수 **/
	UFUNCTION(Server, Reliable)
	void Server_SetMovementMode(const EGate NewGate);

	/** 현재 장착중인 무기가 변경되면 호출하는 함수. **/
	void OnEquippedWeaponChanged();

	void OnAbilityInputPressed(const FGameplayTag& InInputTag) const;
	void OnAbilityInputReleased(const FGameplayTag& InInputTag) const;

protected:
	/* begin Actor Interface */
	virtual void BeginPlay() override;
	/* end Actor Interface */

	/** 캐릭터가 현재 장착 중인 무기에 따라 재생할 애니메이션 레이어를 저장하는 Map **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrandNew|Anim Properties")
	TMap<ECombatWeaponType, TSubclassOf<UAnimInstance>> WeaponAnimLayerMap;

	/** 모든 캐릭터에게 공통으로 부여할 어빌리티를 저장하는 데이터 에셋 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrandNew|Gameplay Ability System")
	TSoftObjectPtr<UDataAsset_DefaultPlayerAbilities> DefaultAbilitiesDataAsset;

private:
	// DefaultAbilities 데이터 에셋에 들어있는 기본 어빌리티들을 GAS에 추가하는 함수
	void AddCharacterAbilities() const;

	/* 현재 장착중인 무기의 종류를 나타내는 enum */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentEquippedWeaponType)
	ECombatWeaponType EquippedWeaponType = ECombatWeaponType::Unequipped;

	ECombatWeaponType LastEquippedWeaponType = ECombatWeaponType::Unequipped;

	UFUNCTION()
	void OnRep_CurrentEquippedWeaponType();
	
#pragma region Movement
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGate)
	EGate CurrentGate = EGate::Jogging;

	UFUNCTION()
	void OnRep_CurrentGate();

	/* 이동 타입에 따른 최대 속도, 브레이크 속도들을 설정하는 Map */
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Movement Properties")
	TMap<EGate, FGateSettings> GateSettings;

	void UpdateMovementComponentPrams();

#pragma endregion Movement

#pragma region CameraComponent
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;
#pragma endregion CameraComponent
	
	
};
