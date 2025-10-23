// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"
#include "BrandNewPlayerCharacter.generated.h"

class ABrandNewPickupItem;
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
	virtual void K2_BroadCastCharacterInitialHealth_Implementation() override;
	virtual float GetAttributeValueByTag(const FGameplayTag& AttributeTag) const override;
	virtual void ApplyAddXPEffect(const float XpToAdd) const override;
	virtual void ApplyLevelUpGameplayEffect(const int32 LevelToApply, const int32 RewardAttributePoint) override;
	virtual int32 FindLevelForXP(const int32 InXP) const override;
	virtual int32 GetAttributePointsReward(const int32 LevelToFind) const override;
	virtual void UpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams) override;
	virtual FOnWeaponChangedDelegate& GetWeaponChangedDelegate() override;
	virtual FOnOverlappedItemChangedDelegate& GetOnOverlapChangedDelegate() override;
	virtual float GetRequiredAbilityMana(const FGameplayTag& AbilityTag) const override;
	virtual void RequestSave(const FString& SlotName, const int32 SlotIndex = 1) override;
	virtual void SavePlayerDataForTravel() override;
	virtual void AddOverlappedItem(AActor* OverlappedItem ) override;
	virtual void RemoveOverlappedItem(AActor* OverlappedItem) override;
	virtual void UseConsumptionItem(const int32 SlotIndex) override;
	virtual void UseEquipmentItem(const int32 SlotIndex, const EItemType ItemType) override;
	virtual void RevivePlayerCharacter() override;
	virtual void AddOverlappedNPC(AActor* OverlappedNPC) override;
	virtual void RemoveOverlappedNPC(AActor* EndOverlappedNPC) override;
	virtual void SetCombatWeaponVisible(const bool bIsVisible) override;
	/* end Player Interface */

	/** 캐릭터의 무브먼트 모드를 변경하는 함수 **/
	UFUNCTION(Server, Reliable)
	void Server_RequestUpdateMovementMode(const EGate NewGate);

	/** 현재 장착중인 무기가 변경되면 호출하는 함수. **/
	void OnEquippedWeaponChanged();

	void OnAbilityInputPressed(const FGameplayTag& InInputTag) const;
	void OnAbilityInputReleased(const FGameplayTag& InInputTag) const;

	void InteractIfPossible();
	void CameraScroll(const float InputValue);
	
	/* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Branenew|Delegates")
	FOnAttributeChangedDelegate HealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Branenew|Delegates")
	FOnAttributeChangedDelegate MaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Branenew|Delegates")
	FOnAttributeChangedDelegate ManaChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Branenew|Delegates")
	FOnAttributeChangedDelegate MaxManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Branenew|Delegates")
	FOnAttributeChangedDelegate LevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Branenew|Delegates")
	FOnAttributeChangedDelegate ExperienceChangedDelegate;
	
	FOnWeaponChangedDelegate WeaponChangedDelegate;
	FOnOverlappedItemChangedDelegate OnOverlappedItemChangedDelegate;
	

protected:
	/* begin Actor Interface */
	virtual void BeginPlay() override;
	/* end Actor Interface */
	
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

	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> VitalOverrideEffect;
	
	/* 체력 및 마나 자동 재생 Gameplay Effect */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> RegenerationEffect;

	/* 경험치를 올리기 위한 Gameplay Effect */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> XPAttributeEffect;

	/* 레벨을 올리며 동시에 Attribute Point를 주기 위한 Effect */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> LevelUpEffect;

	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> AttributeUpgradeEffect;

	/* 모든 Attribute에 Set By Caller로 Instance 효과를 주는 이펙트 */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> AllAttributeInstanceEffect;

	/* 무기 착용시 적용할 Effect */
	UPROPERTY(EditAnywhere, Category = "Brandnew|Gameplay Effect")
	TSubclassOf<UGameplayEffect> EquipmentInfiniteEffect;
	
	/** 캐릭터가 현재 장착 중인 무기에 따라 재생할 애니메이션 레이어를 저장하는 Map **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Anim Properties")
	TMap<ECombatWeaponType, TSubclassOf<UAnimInstance>> WeaponAnimLayerMap;

	/** 플레이어에게 부여할 어빌리티를 저장하는 데이터 에셋 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Data Asset")
	TObjectPtr<UDataAsset_DefaultPlayerAbilities> DefaultAbilitiesDataAsset;
	
	/* Attribute와 GameplayTag를 연결하는 데이터 에셋 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Data Asset")
	TObjectPtr<UDataAsset_AttributeInfo> AttributeInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Data Asset")
	TObjectPtr<UDataAsset_LevelUpInfo> LevelUpInfoDataAsset;

	void ZoomInterpTick();
	

private:
	/* Attribute 변화를 바인딩 하는 함수 */
	void BindAttributeDelegates();
	/* HUD 초기화 하고 초기 값 브로드캐스트 하는 함수 */
	void InitHUDAndBroadCastInitialValue() const;

	float CalculateExpPercent() const;

	void MoveCharacterToValidLocation(const FVector& NewLocation);
	FVector GetSafeTeleportLocation(const FVector& NewLocation) const;

	void ReviveCharacter();
	UFUNCTION(Server, Reliable)
	void Server_ReviveCharacter();
	
	/* 부활할때 사용할 마지막으로 저장하거나 로드된 위치. 서버에서만 설정됨. */
	FVector SafeLocation;

	UFUNCTION(Client, Reliable)
	void Client_RecoveryDataAfterMapTravel();
	
	UFUNCTION(Server, Reliable)
	void Server_RecoveryDataAfterMapTravel(const FString& PlayerName);
	
	/* 캐릭터와 오버랩 중인 픽업 아이템들 */
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappedItems;
	
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappedItemsForUI;

	UPROPERTY()
	TArray<AActor*> OverlappedNPCArray;
	
	/* 현재 락온 중인 타겟 액터 */
	TWeakObjectPtr<AActor> CombatTargetActor;

	UFUNCTION(Server, Reliable)
	void Server_RequestUpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams);

	/* 최초 게임 접속시 시네마틱 재생 */
	void PlayFirstEntranceSequence() const;
	UFUNCTION(Client, Reliable)
	void Client_PlayFirstEntranceSequence();
	
	FText GetCurrentTimeText() const;

#pragma region SaveAndLoad
	
	/**
	 * Attribute를 로드하는 함수.
	 * 로드 데이터는 Player Id마다 다르게 설정해야하고 바른 아이디를 가져오기 위해
	 * 클라이언트가 서버로 아이디를 보내고 서버는 이를 받아서 처리
	 */
	UFUNCTION(Server, Reliable)
	void Server_RequestInitCharacterInfo(const FString& PlayerId);

	// 세이브 데이터 유무에 따라 로드 작업 또는 디폴트 캐릭터 정보를 설정.
	void InitializeCharacterInfo(const FString& UniqueId);
	
	// 세이브 데이터로부터 캐릭터 정보 로드
	void LoadCharacterData(const FSaveSlotPrams& SavedDataToApply);

	// 인벤토리 로드하고 장착중인 아이템이 있었으면 이펙트 적용하는 함수
	void LoadInventory(const FInventoryContents& InventoryData);
	
	/* 세이브 데이터로 부터 캐릭터 정보를 가져오는 함수 */
	void ApplyPrimaryAttributeFromSaveData(const FAttributeSaveData& SlotPrams) const;

	void OverrideVitalAttribute(const float HealthToApply, const float ManaToApply) const;

	/* 데이터 테이블로 부터 캐릭터 정보를 가져오는 함수 */
	void ApplyPrimaryAttributeFromDataTable() const;

	// DefaultAbilities 데이터 에셋에 들어있는 기본 어빌리티들을 GAS에 추가하는 함수
	void AddCharacterAbilities() const;
	
	/* 현재 캐릭터 정보를 바탕으로 세이브 파라메터 구조체를 만드는 함수 */
	FSaveSlotPrams MakeSaveSlotPrams() const;

	// 캐릭터 데이터를 슬롯에 저장
	void SaveCharacterData(const FString& SlotName, int32 SlotIndex, const FString& ClientId);

	UFUNCTION(Client, Reliable)
	void Client_SaveInSlot(const FString& SlotName, const int32 SlotIndex);

	UFUNCTION(Server, Reliable)
	void Server_RequestSave(const FString& SlotName, const int32 SlotIndex, const FString& ClientId);

#pragma endregion

#pragma region Item

	void SendPickupInfoToUi(AActor* ItemToSend, const bool bIsBeginOverlap) const;

	UFUNCTION(Server, Reliable)
	void Server_AcquireItem();
	void AcquireItem();

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(const int32 SlotIndex);
	void ConsumeItem(const int32 SlotIndex) const;

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(const int32 SlotIndex, const EItemType ItemType);
	void EquipItem(const int32 SlotIndex, const EItemType ItemType);

	FActiveGameplayEffectHandle ApplyInfiniteItemEffect(const int32 ItemID) const;

	/* 적용된 장비 효과를 담은 핸들 */
	FActiveGameplayEffectHandle ActiveWeaponEffect;
	FActiveGameplayEffectHandle ActiveArmorEffect;
	
	/* 현재 장착중인 무기의 종류를 나타내는 enum */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentEquippedWeaponType)
	ECombatWeaponType EquippedWeaponType = ECombatWeaponType::Unequipped;

	// 무기 장착 타입에 따라 매핑 컨텍스트를 추가하거나 제거하는데 무기 타입이 바뀔 때 전의 매핑 컨텍스트를 제외하기 위해서 마지막으로 사용한 무기 타입을 저장.
	ECombatWeaponType LastEquippedWeaponType = ECombatWeaponType::Unequipped;

	UFUNCTION()
	void OnRep_CurrentEquippedWeaponType();

#pragma endregion 

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

#pragma region Camera
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Camera", meta=(ClampMin="0.0"))
	float CameraZoomSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Camera")
	float ZoomInterpSpeed = 7.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Camera")
	float MinZoom = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Brandnew|Camera")
	float MaxZoom = 800.f;
	
	FTimerHandle ZoomInterpTimerHandle;
	float TargetArmLength = 500.f;
	bool bIsZooming = false;

#pragma endregion Camera
	
	
};
