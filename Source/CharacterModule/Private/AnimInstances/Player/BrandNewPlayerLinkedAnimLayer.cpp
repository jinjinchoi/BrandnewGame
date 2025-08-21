// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Player/BrandNewPlayerLinkedAnimLayer.h"

#include "AnimInstances/Player/BrandNewPlayerAnimInstance.h"


UBrandNewPlayerAnimInstance* UBrandNewPlayerLinkedAnimLayer::GetPlayerAnimInstance() const
{
	return Cast<UBrandNewPlayerAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
