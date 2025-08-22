// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BrandNewPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Components/BrandNewInputComponent.h"
#include "DataAssets/DataAsset_InputConfig.h"
#include "InputActionValue.h"
#include "Character/BrandNewPlayerCharacter.h"
#include "Interfaces/BrandNewPlayerAnimInterface.h"

void ABrandNewPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(InputConfig);

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);
	}
	
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

		BrandNewInputComponent->BindAbilityInputAction(InputConfig, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
		
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
	ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
	if (!ControlledCharacter) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	ControlledCharacter->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	ControlledCharacter->AddMovementInput(RightDirection, InputAxisVector.X);

	if (IBrandNewPlayerAnimInterface* PlayerAnimInterface = Cast<IBrandNewPlayerAnimInterface>(ControlledCharacter->GetMesh()->GetAnimInstance()))
	{
		PlayerAnimInterface->MovementInputReceived(InputAxisVector, Rotation);
	}
	
	
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
	ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn());
	if (!ControlledCharacter) return;
	ControlledCharacter->Jump();
	
}

void ABrandNewPlayerController::Input_Walk()
{
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->Server_SetMovementMode(EGate::Walking);
	}
}


void ABrandNewPlayerController::Input_Run()
{
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->Server_SetMovementMode(EGate::Running);
	}
}
