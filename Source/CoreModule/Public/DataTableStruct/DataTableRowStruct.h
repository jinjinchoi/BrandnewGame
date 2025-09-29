
#pragma once
#include "BrandNewTypes/BrandNewEnumTypes.h"

#include "DataTableRowStruct.generated.h"


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

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Information")
	int32 ID = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, Category = "Information")
	EItemType ItemType = EItemType::None;

	/* UI에서 보여줄 아이템의 이름 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FName ItemName = FName();

	/* UI에서 보여줄 아이템 설명 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText Description = FText();

	/* UI에서 보여줄 아이템 이미지 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Thumbnail = nullptr;

	/* 슬롯 하나에 최대 보관 가능한 아이템의 수 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StackSize = 1;

	/**
	 * 아이템  사용시 증가시킬 Attribute
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float MaxHealth = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float CurrentHealth = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float MaxMana = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float CurrentMana = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Strength = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Intelligence = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Dexterity = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Vitality = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float PhysicalAttackPower = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float MagicAttackPower = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float PhysicalDefensePower = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float MagicDefensePower = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float CriticalChance = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float CriticalMagnitude = 0.f;
	
};
