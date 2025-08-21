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
struct FAttributeDataTableInfo
{
	GENERATED_BODY()
};