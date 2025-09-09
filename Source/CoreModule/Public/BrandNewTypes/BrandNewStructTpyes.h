#pragma once

#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "BrandNewStructTpyes.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FPlayerAnimData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> DefaultAnimLayerClass = nullptr;
};

/* 캐릭터의 이동 모드에 따른 속도 정의 */
USTRUCT(BlueprintType)
struct FGateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxWalkSpeed = 400.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAcceleration = 2048.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingDecelerationWalking = 2048.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFrictionFactor = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFriction = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseSeparateBrakingFriction = true;
	
};


/* 캐릭터 Attribute를 설정하기 위한 데이터 테이블을 구성할 row 구조체 */
USTRUCT(BlueprintType)
struct FPrimaryAttributeDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName ID = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Strength = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Intelligence = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Dexterity = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Vitality = 0.f;
	
};

/* 캐릭터 1차 속성을 설정하기 위한 구조체 */
struct FBaseAttributePrams
{
	float Strength = 0.f;
	float Intelligence = 0.f;
	float Dexterity = 0.f;
	float Vitality = 0.f;
	float Level = 1.f;
};

/* Enemy의 Attribute를 저장할 데이터 테이블 Row */
USTRUCT(BlueprintType)
struct FSecondaryAttributeDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName ID = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float PhysicalAttackPower = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float MagicAttackPower = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float PhysicalDefensePower = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float MagicDefensePower = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float CriticalChance = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float CriticalMagnitude = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float MaxHealth = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Secondary Attribute")
	float MaxMana = 0.f;
	
};


/* 2차 속성을 저장하는 구조체 */
struct FSecondaryAttributePrams
{
	float PhysicalAttackPower = 0.f;
	float MagicAttackPower = 0.f;
	float PhysicalDefensePower = 0.f;
	float MagicDefensePower = 0.f;
	float CriticalChance = 0.f;
	float CriticalMagnitude = 0.f;
	float MaxHealth = 0.f;
	float CurrentHealth = 0.f;
	float MaxMana = 0.f;
	float CurrentMana = 0.f;
};


USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	FGameplayTag DamageType = FGameplayTag();
	FGameplayTag DamageElement = FGameplayTag();
	FGameplayTag HitDirection = FGameplayTag();
	int32 AbilityLevel = 1;
	float DamageCoefficient = 0.f;
	FVector KnockbackImpulse = FVector::ZeroVector;

	bool IsValid () const
	{
		return WorldContextObject && SourceAbilitySystemComponent && TargetAbilitySystemComponent && DamageGameplayEffectClass;
	}
};

