// Fill out your copyright notice in the Description page of Project Settings.


#include "UIFunctionLibrary.h"

#include "HUD/BrandNewHUD.h"

UCharacterInfoWidgetController* UUIFunctionLibrary::GetCharacterInfoWidgetController(const UObject* WorldContextObject)
{
	if (const APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (ABrandNewHUD* HUD = Cast<ABrandNewHUD>(PlayerController->GetHUD()))
		{
			return HUD->GetCharacterInfoWidgetController();
		}
	}

	return nullptr;
}
