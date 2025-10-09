// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerController.h"

#include "CharacterFunctionLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Components/BrandNewInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "Character/BrandNewPlayerCharacter.h"
#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "Interfaces/Animation/BrandNewPlayerAnimInterface.h"
#include "Player/BrandNewPlayerState.h"

ABrandNewPlayerController::ABrandNewPlayerController()
{
	PlayerTeamID = TEAM_PLAYER;
}


void ABrandNewPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(InputConfig);

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		const ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
		if (!ControlledCharacter) return;
		
		if (UInputMappingContext** FoundMappingContext = InputConfig->MappingContextMap.Find(ControlledCharacter->GetCurrentEquippedWeaponType()))
		{
			SubSystem->AddMappingContext(*FoundMappingContext, 0);
		}
	}
	
}


FGenericTeamId ABrandNewPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}

void ABrandNewPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// Server
	PlayerInterface = InPawn;
	check(PlayerInterface);

	UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;
	
	ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	if (!BrandNewPlayerState) return;
	BrandNewPlayerState->SetPlayerNameToPlayerState(SaveSubsystem->GetUniqueIdentifier());
	
	
}

void ABrandNewPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	// Client
	PlayerInterface = GetPawn();

	UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;
	
	ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	if (!BrandNewPlayerState) return;
	BrandNewPlayerState->SetPlayerNameToPlayerState(SaveSubsystem->GetUniqueIdentifier());
	
}

void ABrandNewPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UBrandNewInputComponent* BrandNewInputComponent = Cast<UBrandNewInputComponent>(InputComponent))
	{
		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_Move, ETriggerEvent::Triggered, &ThisClass::Input_Move);
		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_Look, ETriggerEvent::Triggered, &ThisClass::Input_Look);
		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_Run, ETriggerEvent::Started, &ThisClass::Input_Run);
		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_Walk, ETriggerEvent::Started, &ThisClass::Input_Walk);
		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_Jump, ETriggerEvent::Triggered, &ThisClass::Input_Jump);
		
		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_Interact, ETriggerEvent::Triggered, &ThisClass::Input_Interact);

		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_UI_OpenInGameMenu, ETriggerEvent::Triggered, &ThisClass::Input_OpenInGameMenu);

		BrandNewInputComponent->BindAbilityInputAction(InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
		
	}
	
}

void ABrandNewPlayerController::AddInputMappingForWeapon(const ECombatWeaponType InWeaponType)
{
	if (InWeaponType <= ECombatWeaponType::Unequipped) return;
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!SubSystem) return;
	
	if (UInputMappingContext** FoundMappingContext = InputConfig->MappingContextMap.Find(InWeaponType))
	{
		SubSystem->AddMappingContext(*FoundMappingContext, 0);
	}
}

void ABrandNewPlayerController::RemoveInputMappingForWeapon(const ECombatWeaponType WeaponTypeToRemove)
{
	if (WeaponTypeToRemove <= ECombatWeaponType::Unequipped) return;

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!SubSystem) return;
	
	if (UInputMappingContext** FoundMappingContext = InputConfig->MappingContextMap.Find(WeaponTypeToRemove))
	{
		SubSystem->RemoveMappingContext(*FoundMappingContext);
	}
	
}

void ABrandNewPlayerController::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	const ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
	if (!ControlledCharacter) return;

	ControlledCharacter->OnAbilityInputPressed(InInputTag);
}

void ABrandNewPlayerController::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	const ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
	if (!ControlledCharacter) return;

	ControlledCharacter->OnAbilityInputReleased(InInputTag);
}

void ABrandNewPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!PlayerInterface || PlayerInterface->IsHitReacting()) return;
	
	ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
	if (!ControlledCharacter) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	ControlledCharacter->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	ControlledCharacter->AddMovementInput(RightDirection, InputAxisVector.X);

	// for turn in place
	// if (IBrandNewPlayerAnimInterface* PlayerAnimInterface = Cast<IBrandNewPlayerAnimInterface>(ControlledCharacter->GetMesh()->GetAnimInstance()))
	// {
	// 	PlayerAnimInterface->MovementInputReceived(InputAxisVector, Rotation);
	// }
	
	
}

void ABrandNewPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddPitchInput(LookAxisVector.Y);
	}
}

void ABrandNewPlayerController::Input_Jump()
{
	if (!PlayerInterface || PlayerInterface->IsHitReacting()) return;
	
	ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
	if (!ControlledCharacter) return;
	ControlledCharacter->Jump();
	
}

void ABrandNewPlayerController::Input_Walk()
{
	if (UCharacterFunctionLibrary::DoseActorHasTag(GetPawn(), BrandNewGamePlayTag::Block_Player_GageChange)) return;
	
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->Server_RequestUpdateMovementMode(EGate::Walking);
	}
}


void ABrandNewPlayerController::Input_Run()
{
	if (UCharacterFunctionLibrary::DoseActorHasTag(GetPawn(), BrandNewGamePlayTag::Block_Player_GageChange)) return;
	
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->Server_RequestUpdateMovementMode(EGate::Running);
	}
}

void ABrandNewPlayerController::Input_Interact()
{
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->InteractIfPossible();
	}
}

void ABrandNewPlayerController::Input_OpenInGameMenu()
{
	if (!IsLocalController()) return;
	
	check(InGameMenuWidgetClass);
	
	if (!InGameMenuWidget)
	{
		InGameMenuWidget = CreateWidget(this, InGameMenuWidgetClass);
	}

	InGameMenuWidget->AddToViewport();
	
}
