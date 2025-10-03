// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "UObject/Interface.h"
#include "BrandNewPlayerInterface.generated.h"

struct FGameplayTag;
DECLARE_DELEGATE_OneParam(FOnAttributeChangedDelegate, const float);
DECLARE_DELEGATE_OneParam(FOnWeaponChangedDelegate, const ECombatWeaponType);
DECLARE_DELEGATE_TwoParams(FOnOverlappedItemChangedDelegate, const bool /* Is Begin Overlap? */, const FPickupsUiInfo&)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBrandNewPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBrandNewPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* 현재 장착 중인 무기의 타입을 반환하는 함수 */
	virtual ECombatWeaponType GetCurrentEquippedWeaponType () const = 0;
	
	/* Turn In Place 애니메이션 커브에서 값 가져와서 캐릭터 회전시키는 함수 */
	virtual void AddYawRotation(const float DeltaYaw) = 0;
	
	/* 캐릭터 클래스에 있는 Combat Weapon Actor를 가져오는 함수. 해당 무기의 멤버변수에 접근하려면 Cast<ABrandNewWeapon> 해야함 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatWeaponActor() const;

	/* 무기를 장착하면 플레이어의 장착중인 무기 타입 enum을 변경하기 위해 호출하는 함수 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnWeaponEquipped();

	/* 무장 해제시 무기 타입 enum을 Unequip으로 변경하기 위해 별도의 함수로 분리 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnWeaponUnequipped();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetStrafeState();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetFireMode(const bool IsFiring);

	/* 락온 시 락온 중인 대상을 저장하는 함수. 해당 데이터는 서버만 가지고 있음. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTargetActor(AActor* TargetActor);

	/* 락온 시 락온 중인 대상을 가져오는 함수. 액터 포인터는 서버에서만 유효함. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTargetActor();
	
	/* Vital Attribute 값을 브로드캐스트 요청 하는 함수 */
	virtual void RequestBroadCastAttributeValue() = 0;

	/* 경험치를 더하는 함수 */
	virtual void ApplyAddXPEffect(const float XpToAdd) const = 0;
	virtual int32 FindLevelForXP(const int32 InXP) const = 0;
	virtual int32 GetAttributePointsReward(int32 LevelToFind) const = 0;
	virtual void ApplyLevelUpGameplayEffect(const int32 LevelToApply, const int32 RewardAttributePoint) = 0;

	virtual FOnAttributeChangedDelegate& GetHealthChangedDelegate() = 0;
	virtual FOnAttributeChangedDelegate& GetMaxHealthChangedDelegate() = 0;
	virtual FOnAttributeChangedDelegate& GetManaChangedDelegate() = 0;
	virtual FOnAttributeChangedDelegate& GetMaxManaChangedDelegate() = 0;
	virtual FOnWeaponChangedDelegate& GetWeaponChangedDelegate() = 0;
	virtual FOnOverlappedItemChangedDelegate& GetOnOverlapChangedDelegate() = 0;
	
	// 태그를 통해 Attribute의 value를 가져오는 함수
	virtual float GetAttributeValueByTag(const FGameplayTag& AttributeTag) const = 0;
	virtual void UpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams) = 0;

	virtual float GetRequiredAbilityMana(const FGameplayTag& AbilityTag) const = 0;

	virtual void RequestSave(const FString& SlotName, const int32 SlotIndex = 1) = 0;
	
	virtual void AddOverlappedItem(AActor* OverlappedItem) = 0;
	virtual void RemoveOverlappedItem(AActor* OverlappedItem) = 0;

	/* Instance Gameplay Effect를 사용하여 아이템 효과에 해당하는 Attribute를 영구적으로 증가 시킴 */
	virtual void UseConsumptionItem(const int32 SlotIndex) = 0;
	virtual void UseEquipmentItem(const int32 SlotIndex, const EItemType ItemType) = 0;
};
