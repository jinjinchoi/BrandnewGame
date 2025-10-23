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
#include "Interfaces/Player/BnPlayerStateInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/UI/EntryStatusWidgetInterface.h"

ABrandNewPlayerController::ABrandNewPlayerController()
{
	PlayerTeamID = TEAM_PLAYER;
}


FGenericTeamId ABrandNewPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
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

void ABrandNewPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsLocalController()) return;
	
	// Server
	PlayerInterface = InPawn;
	check(PlayerInterface);

	const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;
	
	SetPlayerIdToPlayerState(SaveSubsystem->GetUniqueIdentifier());
	
}

void ABrandNewPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (!IsLocalController()) return;

	// Client
	PlayerInterface = GetPawn();

	const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;

	Server_SetPlayerIdToPlayerState(SaveSubsystem->GetUniqueIdentifier());
	
}

void ABrandNewPlayerController::Server_SetPlayerIdToPlayerState_Implementation(const FString& ClientName)
{
	SetPlayerIdToPlayerState(ClientName);
}

void ABrandNewPlayerController::SetPlayerIdToPlayerState(const FString& PlayerName) const
{
	IBnPlayerStateInterface* PlayerStateInterface = Cast<IBnPlayerStateInterface>(PlayerState);
	if (!PlayerStateInterface) return;
	
	PlayerStateInterface->SetPlayerNameToPlayerState(PlayerName);
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

		BrandNewInputComponent->BindLocomotionInputAction(
			InputConfig, this, BrandNewGamePlayTag::Input_CameraScroll, ETriggerEvent::Triggered, &ThisClass::Input_CameraScroll);

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

void ABrandNewPlayerController::HandlePlayerMapEntryOverlap(const int32 OverlappedPlayersNum, const int32 MaxPlayersNum)
{
	check(EntryStatusWidgetClass)
	
	if (!EntryStatusWidgetClass) return;

	// 이미 로컬 컨트롤러에서만 호출되도록 설정되어 있지만 가독성 향상을 위해 if 체크  
	if (IsLocalController())
	{
		CreateOrUpdateEntryStatusWidget(OverlappedPlayersNum, MaxPlayersNum);
	}
	
}

void ABrandNewPlayerController::CreateOrUpdateEntryStatusWidget(const int32 OverlappedPlayersNum, const int32 MaxPlayersNum)
{
	// 현재 오버랩 된 플레이어가 없으면 위젯 제거
	if (OverlappedPlayersNum <= 0 && EntryStatusWidget)
	{
		EntryStatusWidget->RemoveFromParent();
		EntryStatusWidget = nullptr;
		return;
	}

	// 엔트리 정보 위젯 생성 및 가운데에서 Y Off 준 위치에 정렬
	if (!EntryStatusWidget)
	{
		EntryStatusWidget = CreateWidget(this, EntryStatusWidgetClass);
		
		EntryStatusWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		EntryStatusWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.2f, 0.5f, 0.2f));
		
		EntryStatusWidget->AddToViewport();
		
	}

	// 오버랩 된 플레이어의 수와 전체 플레이어의 수 전송
	if (EntryStatusWidget->Implements<UEntryStatusWidgetInterface>())
	{
		IEntryStatusWidgetInterface::Execute_SendCurrentOverlappedPlayersNum(EntryStatusWidget, OverlappedPlayersNum);
		IEntryStatusWidgetInterface::Execute_SendMaxPlayersNum(EntryStatusWidget, MaxPlayersNum);
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

bool ABrandNewPlayerController::CanNotInteraction() const
{
	return !PlayerInterface || PlayerInterface->IsHitReacting() || IBrandNewCharacterInterface::Execute_IsDead(GetPawn());
}


void ABrandNewPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	if (CanNotInteraction()) return;
	
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
	if (CanNotInteraction()) return;
	
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
	if (CanNotInteraction()) return;
	
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->InteractIfPossible();
	}
}

void ABrandNewPlayerController::Input_CameraScroll(const FInputActionValue& InputActionValue)
{
	if (ABrandNewPlayerCharacter* ControlledCharacter = Cast<ABrandNewPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->CameraScroll(InputActionValue.Get<float>());
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
