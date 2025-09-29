// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "UObject/Interface.h"
#include "BrandNewPlayerAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBrandNewPlayerAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREMODULE_API IBrandNewPlayerAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* 애님인스턴스에 존재하는 Gate 변수를 업데이트하는 함수 */
	virtual void UpdateCurrentGate(const EGate NewGate) = 0;
	/* 캐릭터 이동시 현재 회전 값과 마지막 입력 값 넘겨주는 함수 */
	virtual void MovementInputReceived(const FVector2D& LastInputVector, const FRotator& ControllerRotation) = 0;
	/* 캐릭터가 공중에 있을때 땅과의 거리를 보내는 함수 */
	virtual void ReceiveGroundDistance(const float InDistance) = 0;
};
