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


/* 캐릭터 1차 속성을 설정하기 위한 구조체 */
struct FBaseAttributePrams
{
	float Strength = 0.f;
	float Intelligence = 0.f;
	float Dexterity = 0.f;
	float Vitality = 0.f;
	float Level = 1.f;
	float AttributePoint = 0.f;
	float XP = 0.f;
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

/* Attribute를 강화할때 어떤 Attribute를 얼마만큼 할지 저장하는 구조체 */
USTRUCT(BlueprintType)
struct FAttributeUpgradePrams
{
	GENERATED_BODY()
	
	FAttributeUpgradePrams() {}
	FAttributeUpgradePrams(const FGameplayTag& InTag, const int32 Value)
		: TagToUpgrade(InTag), UpgradeAmount(Value) {}

	UPROPERTY()
	FGameplayTag TagToUpgrade = FGameplayTag();
	
	UPROPERTY()
	int32 UpgradeAmount = 0;
	
};

/* 로딩화면에서 보여줄 팁을 저장하는 데이터 테이블을 위한 구조체 */
USTRUCT(BlueprintType)
struct FTipData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Brandnew|Tip")
	FText TipText;
	
};

/* 세이브 데이터 중 Attribute 데이터를 저장하는 구조체 */
USTRUCT()
struct FAttributeSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	float Strength = 0.f;
	UPROPERTY()
	float Intelligence = 0.f;
	UPROPERTY()
	float Dexterity = 0.f;
	UPROPERTY()
	float Vitality = 0.f;
	UPROPERTY()
	float Level = 1.f;
	UPROPERTY()
	float Experience = 0.f;
	UPROPERTY()
	float AttributePoint = 0.f;
	UPROPERTY()
	float CurrentHealth = 0.f;
	UPROPERTY()
	float CurrentMana = 0.f;
};

/* 모든 세이버 데이터를 종합하는 구조체 */
struct FSaveSlotPrams
{
	
	/* Attribute 정보 */
	FAttributeSaveData AttributePrams = FAttributeSaveData();
	
	/* 캐릭터 위치 */
	FVector CharacterLocation = FVector::ZeroVector;
	
	/* Map이름 */
	FText MapName = FText::GetEmpty();
	
	/* 어빌리티 태그와 레벨이 저장되는 Map */
	TMap<FGameplayTag, int32 > AbilityMap;
	
	/* 세이브 시간 */
	FText SavedTime = FText::GetEmpty();
	
	/*세이브 슬롯 제목에 쓰일 Text*/
	FText TitleText = FText::GetEmpty();

	/* Open World에서 사용할 Map Name */
	FString MapPackageName = FString(); 

	bool bIsValid = false;
};