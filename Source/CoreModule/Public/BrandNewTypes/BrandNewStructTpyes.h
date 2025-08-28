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
	
	UPROPERTY(EditDefaultsOnly, Category = "Vital Attribute")
	float MaxHealth = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Vital Attribute")
	float MaxMana = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Strength = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Intelligence = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Dexterity = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Primary Attribute")
	float Vitality = 0.f;
	
};

/* 캐릭터 Attribute를 설정하기 위한 구조체 */
struct FBaseAttributePrams
{
	float MaxHealth = 0.f;
	float CurrentHealth = 0.f;
	float MaxMana = 0.f;
	float CurrentMana = 0.f;
	
	float Strength = 0.f;
	float Intelligence = 0.f;
	float Dexterity = 0.f;
	float Vitality = 0.f;
	
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
	int32 AbilityLevel = 1;
	float DamageCoefficient = 0.f;

	bool IsValid () const
	{
		return WorldContextObject && SourceAbilitySystemComponent && TargetAbilitySystemComponent && DamageGameplayEffectClass;
	}
};

