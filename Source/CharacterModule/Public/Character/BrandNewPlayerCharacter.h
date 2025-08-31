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
	virtual FOnAttributeChangedDelegate& GetHealthChangedDelegate() override;
	virtual FOnAttributeChangedDelegate& GetMaxHealthChangedDelegate() override;
	virtual FOnAttributeChangedDelegate& GetManaChangedDelegate() override;
	virtual FOnAttributeChangedDelegate& GetMaxManaChangedDelegate() override;
	virtual void RequestBroadCastAttributeValue() override;
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

	/* begin Base Character */
	virtual void InitAbilityActorInfo() override;
	/* end Base Character */
	void ApplyPrimaryAttribute() const;

	/* 캐릭터의 최초 Attribute가 저장되어 있는 데이터 테이블 */
	UPROPERTY(EditAnywhere, Category = "BrandNew|DataTable")
	TObjectPtr<UDataTable> AttributeDataTable;

	/* AttributeDataTable의 Key Name */
	UPROPERTY(EditAnywhere, Category = "BrandNew|DataTable")
	FName AttributeTableKeyName;
	
	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSubclassOf<UGameplayEffect> PrimaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "BrandNew|Gameplay Ability System")
	TSubclassOf<UGameplayEffect> VitalAttributeEffect;

	/** 플레이어에게 부여할 어빌리티를 저장하는 데이터 에셋 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrandNew|Gameplay Ability System")
	TSoftObjectPtr<UDataAsset_DefaultPlayerAbilities> DefaultAbilitiesDataAsset;

	/** 캐릭터가 현재 장착 중인 무기에 따라 재생할 애니메이션 레이어를 저장하는 Map **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrandNew|Anim Properties")
	TMap<ECombatWeaponType, TSubclassOf<UAnimInstance>> WeaponAnimLayerMap;


private:
	// DefaultAbilities 데이터 에셋에 들어있는 기본 어빌리티들을 GAS에 추가하는 함수
	void AddCharacterAbilities() const;
	/* Attribute 변화를 바인딩 하는 함수 */
	void BindAttributeDelegates();
	/* HUD 초기화 하고 초기 값 브로드캐스트 하는 함수 */
	void InitHUDAndBroadCastInitialValue() const;

	/* 현재 장착중인 무기의 종류를 나타내는 enum */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentEquippedWeaponType)
	ECombatWeaponType EquippedWeaponType = ECombatWeaponType::Unequipped;

	// 무기 장착 타입에 따라 매핑 컨텍스트를 추가하거나 제거하는데 무기 타입이 바뀔 때 전의 매핑 컨텍스트를 제외하기 위해서 마지막으로 사용한 무기 타입을 저장.
	ECombatWeaponType LastEquippedWeaponType = ECombatWeaponType::Unequipped;

	UFUNCTION()
	void OnRep_CurrentEquippedWeaponType();

	FOnAttributeChangedDelegate HealthChangedDelegate;
	FOnAttributeChangedDelegate MaxHealthChangedDelegate;
	FOnAttributeChangedDelegate ManaChangedDelegate;
	FOnAttributeChangedDelegate MaxManaChangedDelegate;
	
	
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
