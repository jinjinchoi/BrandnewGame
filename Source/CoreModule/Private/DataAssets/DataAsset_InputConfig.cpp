// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FInputActionConfig& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.IsValid() && InputActionConfig.InputTag.MatchesTagExact(InInputTag))
		{
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
	
}
