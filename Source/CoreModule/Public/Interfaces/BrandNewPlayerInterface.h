// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "UObject/Interface.h"
#include "BrandNewPlayerInterface.generated.h"

struct FGameplayTag;
DECLARE_DELEGATE_OneParam(FOnAttributeChangedDelegate, const float);

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

	/* Vital Attribute 값을 브로드캐스트 요청 하는 함수 */
	virtual void RequestBroadCastAttributeValue() = 0;

	virtual FOnAttributeChangedDelegate& GetHealthChangedDelegate() = 0;
	virtual FOnAttributeChangedDelegate& GetMaxHealthChangedDelegate() = 0;
	virtual FOnAttributeChangedDelegate& GetManaChangedDelegate() = 0;
	virtual FOnAttributeChangedDelegate& GetMaxManaChangedDelegate() = 0;

	virtual float GetAttributeByTag(const FGameplayTag& AttributeTag) const = 0;
};
