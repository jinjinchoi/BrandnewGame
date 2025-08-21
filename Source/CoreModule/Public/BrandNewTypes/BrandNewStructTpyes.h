#pragma once

#include "BrandNewStructTpyes.generated.h"

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


/* 캐릭터 Attribute를 설정하기 위한 데이터 테이블용 구조체 */
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

/* 캐릭터 Attribute를 설정하기 위한 데이터 테이블용 구조체 */
USTRUCT(BlueprintType)
struct FBaseAttributePrams
{
	GENERATED_BODY()


	float MaxHealth = 0.f;
	float CurrentHealth = 0.f;
	float MaxMana = 0.f;
	float CurrentMana = 0.f;
	
	float Strength = 0.f;
	float Intelligence = 0.f;
	float Dexterity = 0.f;
	float Vitality = 0.f;
	
};