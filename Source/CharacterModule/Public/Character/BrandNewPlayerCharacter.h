// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "Interfaces/BrandNewPlayerInterface.h"
#include "BrandNewPlayerCharacter.generated.h"

class UDataAsset_LevelUpInfo;
class UDataAsset_AttributeInfo;
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
	virtual AActor* GetCombatWeaponActor_Implementation() const override;
	virtual void OnWeaponEquipped_Implementation() override;
	virtual void OnWeaponUnequipped_Implementation() override;
	virtual void SetStrafeState_Implementation() override;
	virtual void SetFireMode_Implementation(const bool IsFiring) override;
	virtual void SetCombatTargetActor_Implementation(AActor* TargetActor) override;
	virtual AActor* GetCombatTargetActor_Implementation() override;
	virtual void RequestBroadCastAttributeValue() override;
	virtual float GetAttributeValueByTag(const FGameplayTag& AttributeTag) const override;
	virtual void ApplyAddXPEffect(const float XpToAdd) const override;
	virtual void ApplyLevelUpGameplayEffect(const int32 LevelToApply, const int32 RewardAttributePoint) override;
	virtual int32 FindLevelForXP(const int32 InXP) const override;
	virtual int32 GetAttributePointsReward(const int32 LevelToFind) const override;
	virtual void UpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams) override;
	virtual FOnAttributeChangedDelegate& GetHealthChangedDelegate() override;
	virtual FOnAttributeChangedDelegate& GetMaxHealthChangedDelegate() override;
	virtual FOnAttributeChangedDelegate& GetManaChangedDelegate() override;
	virtual FOnAttributeChangedDelegate& GetMaxManaChangedDelegate() override;
	virtual FOnWeaponChangedDelegate& GetWeaponChangedDelegate() override;
	virtual float GetRequiredAbilityMana(const FGameplayTag& AbilityTag) const override;
	virtual void RequestSave(const FString& SlotName, const int32 SlotIndex = 1) override;
	/* end Player Interface */

	/** 캐릭터의 무브먼트 모드를 변경하는 함수 **/
	UFUNCTION(Server, Reliable)
	void Server_RequestUpdateMovementMode(const EGate NewGate);

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
	UPROPERTY(EditAnywhere, Category = "Brandnew|DataTable")
	TObjectPtr<UDataTable> AttributeDataTable;

	/* AttributeDataTable의 Key Name */
	UPROPERTY(EditAnywhere, Category = "Brandnew|DataTable")
	FName AttributeTableKeyName;
	
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> PrimaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> VitalAttributeEffect;

	/* 경험치를 올리기 위한 Gameplay Effect */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> XPAttributeEffect;

	/* 레벨을 올리며 동시에 Attribute Point를 주기 위한 Effect */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> LevelUpEffect;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> AttributeUpgradeEffect;
	
	/** 캐릭터가 현재 장착 중인 무기에 따라 재생할 애니메이션 레이어를 저장하는 Map **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Anim Properties")
	TMap<ECombatWeaponType, TSubclassOf<UAnimInstance>> WeaponAnimLayerMap;

	/** 플레이어에게 부여할 어빌리티를 저장하는 데이터 에셋 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Data Asset")
	TSoftObjectPtr<UDataAsset_DefaultPlayerAbilities> DefaultAbilitiesDataAsset;
	
	/* Attribute와 GameplayTag를 연결하는 데이터 에셋 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Data Asset")
	TObjectPtr<UDataAsset_AttributeInfo> AttributeInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Data Asset")
	TObjectPtr<UDataAsset_LevelUpInfo> LevelUpInfoDataAsset;
	

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

	UFUNCTION(Server, Reliable)
	void Server_RequestUpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams);

	FText GetCurrentTimeText() const;

	FOnAttributeChangedDelegate HealthChangedDelegate;
	FOnAttributeChangedDelegate MaxHealthChangedDelegate;
	FOnAttributeChangedDelegate ManaChangedDelegate;
	FOnAttributeChangedDelegate MaxManaChangedDelegate;
	FOnWeaponChangedDelegate WeaponChangedDelegate;
	
	TWeakObjectPtr<AActor> CombatTargetActor;
	
#pragma region Movement
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGate)
	EGate CurrentGate = EGate::Jogging;
	
	EGate LastGate = EGate::Jogging;

	UFUNCTION()
	void OnRep_CurrentGate();

	/* 이동 타입에 따른 최대 속도, 브레이크 속도들을 설정하는 Map */
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Movement Properties")
	TMap<EGate, FGateSettings> GateSettings;

	void UpdateMovementComponentPrams();
	void SetMovementMode(const EGate NewGate);

#pragma endregion Movement

#pragma region CameraComponent
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;
#pragma endregion CameraComponent
	
	
};
