// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/BnPlayerControllerInterface.h"
#include "BrandNewPlayerController.generated.h"

struct FInputActionValue;
class UDataAsset_InputConfig;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerController : public APlayerController, public IBnPlayerControllerInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/* begin IBnPlayerControllerInterface */
	virtual void AddInputMappingForWeapon(const ECombatWeaponType InWeaponType) override;
	virtual void RemoveInputMappingForWeapon(const ECombatWeaponType WeaponTypeToRemove) override;
	/* end IBnPlayerControllerInterface */

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input Properties")
	TObjectPtr<UDataAsset_InputConfig> InputConfig;

	/* Input */
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump();
	
	void Input_Walk();
	void Input_Run();
};
