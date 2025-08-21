// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewBaseCharacter.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Interfaces/BrandNewPlayerInterface.h"
#include "BrandNewPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerCharacter : public ABrandNewBaseCharacter, public IBrandNewPlayerInterface
{
	GENERATED_BODY()

public:
	ABrandNewPlayerCharacter();
	virtual void Tick(float DeltaTime) override;

	/* begin Player Interface */
	virtual EEquippedWeapon GetCurrentEquippedWeaponType() const override;
	virtual void AddYawRotation(const float DeltaYaw) override;
	/* end Player Interface */

	/* 캐릭터의 무브먼트 모드를 변경하는 함수 */
	void SetMovementMode(const EGate NewGate);

	/** 현재 장착중인 무기가 변경되면 호출하는 함수. **/
	void OnEquippedWeaponChanged();

protected:
	/* begin Actor Interface */
	virtual void BeginPlay() override;
	/* end Actor Interface */

	/* 캐릭터가 현재 장착 중인 무기에 따라 재생할 애니메이션 레이어를 저장하는 Map */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BrandNew|Anim Properties")
	TMap<EEquippedWeapon, TSubclassOf<UAnimInstance>> WeaponAnimLayerMap;

private:

#pragma region Movement
	
	EEquippedWeapon CurrentEquippedWeaponType = EEquippedWeapon::Unequipped;
	EGate CurrentGate = EGate::Jogging;

	/* 이동 타입에 따른 최대 속도, 브레이크 속도들을 설정하는 Map */
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Movement Properties")
	TMap<EGate, FGateSettings> GateSettings;

#pragma endregion Movement


#pragma region CameraComponent
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;
#pragma endregion CameraComponent
	
	
};
