// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Player/BnPlayerControllerInterface.h"
#include "BrandNewPlayerController.generated.h"

class IBrandNewCharacterInterface;
struct FInputActionValue;
class UDataAsset_InputConfig;
/**
 * 
 */
UCLASS()
class CHARACTERMODULE_API ABrandNewPlayerController : public APlayerController, public IBnPlayerControllerInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ABrandNewPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

	/* begin IBnPlayerControllerInterface */
	virtual void AddInputMappingForWeapon(const ECombatWeaponType InWeaponType) override;
	virtual void RemoveInputMappingForWeapon(const ECombatWeaponType WeaponTypeToRemove) override;
	/* end IBnPlayerControllerInterface */

	/* begin IGenericTeamAgentInterface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/* end IGenericTeamAgentInterface */



private:
	FGenericTeamId PlayerTeamID;


#pragma region Input
	
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|Input Properties")
	TObjectPtr<UDataAsset_InputConfig> InputConfig;

	UPROPERTY()
	TScriptInterface<IBrandNewCharacterInterface> PlayerInterface;

	/* Input */
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump();
	
	void Input_Walk();
	void Input_Run();

	void Input_Interact();

#pragma endregion
	
////////////////////////////////////
///////////  UI  //////////////////
///////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "BrandNew|UI")
	TSubclassOf<UUserWidget> InGameMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> InGameMenuWidget;

	void Input_OpenInGameMenu();
	
	
};
