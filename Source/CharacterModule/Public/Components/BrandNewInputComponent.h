// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "CoreModule/Public/DataAssets/DataAsset_InputConfig.h"
#include "BrandNewInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API UBrandNewInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename CallbackFunc>
	void BindLocomotionInputAction(const UDataAsset_InputConfig* InInputConfig, UserClass* ContextObject, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, CallbackFunc Func);

	template<class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
	
	
	
};

template <class UserClass, typename CallbackFunc>
void UBrandNewInputComponent::BindLocomotionInputAction(const UDataAsset_InputConfig* InInputConfig,
	UserClass* ContextObject, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, CallbackFunc Func)
{
	check(InInputConfig);
	
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}

}

template <class UserObject, typename CallbackFunc>
void UBrandNewInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,
	UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	check(InInputConfig);
	
	for (const FInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid()) continue;

		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputActionConfig.InputTag);
		BindAction(AbilityInputActionConfig.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputActionConfig.InputTag);
	}	
}
