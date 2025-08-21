// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/BrandNewPlayerAnimInterface.h"
#include "Net/UnrealNetwork.h"


ABrandNewPlayerCharacter::ABrandNewPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 65.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
}

void ABrandNewPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling())
	{
		const FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		const FVector End = Start  - FVector(0.f, 0.f, 100);
		constexpr float Radius = 5.f;
		FHitResult HitResult;
		const bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult, Start, End, FQuat::Identity, ECC_Visibility,  FCollisionShape::MakeSphere(Radius));

		if (!bHit) return;
		
		if (IBrandNewPlayerAnimInterface* PlayerAnimInterface = Cast<IBrandNewPlayerAnimInterface>(GetMesh()->GetAnimInstance()))
		{
			PlayerAnimInterface->ReceiveGroundDistance(HitResult.Distance);
		}


#if WITH_EDITOR // === 디버그 라인 ===
		
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0.f, 0, 2.f);
		DrawDebugSphere(GetWorld(), End, Radius, 16, LineColor, false, 0.f);

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 16, FColor::Yellow, false, 2.f);
		}
		
#endif
		
	}
	
}

void ABrandNewPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentEquippedWeaponType);
	DOREPLIFETIME(ThisClass, CurrentGate);
	
}

void ABrandNewPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnEquippedWeaponChanged();


	SetMovementMode(CurrentGate);

}


EEquippedWeapon ABrandNewPlayerCharacter::GetCurrentEquippedWeaponType() const
{
	return CurrentEquippedWeaponType;
}

void ABrandNewPlayerCharacter::AddYawRotation(const float DeltaYaw)
{
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw += DeltaYaw;
	SetActorRotation(NewRot);
}


void ABrandNewPlayerCharacter::SetMovementMode_Implementation(const EGate NewGate)
{
	if (!HasAuthority()) return;
	
	CurrentGate = (CurrentGate == NewGate) ? EGate::Jogging : NewGate;
	
	UpdateMovementComponentPrams();

	if (IBrandNewPlayerAnimInterface* AnimInterface = Cast<IBrandNewPlayerAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->UpdateCurrentGate(CurrentGate);
	}
	
}

void ABrandNewPlayerCharacter::OnRep_CurrentGate()
{
	UpdateMovementComponentPrams();
	
	if (IBrandNewPlayerAnimInterface* AnimInterface = Cast<IBrandNewPlayerAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->UpdateCurrentGate(CurrentGate);
	}
}

void ABrandNewPlayerCharacter::UpdateMovementComponentPrams()
{
	if (GateSettings.Contains(CurrentGate))
	{
		const FGateSettings GateSettingsToApply = GateSettings[CurrentGate];

		GetCharacterMovement()->MaxWalkSpeed = GateSettingsToApply.MaxWalkSpeed;
		GetCharacterMovement()->MaxAcceleration = GateSettingsToApply.MaxAcceleration;
		GetCharacterMovement()->BrakingDecelerationWalking = GateSettingsToApply.BrakingDecelerationWalking;
		GetCharacterMovement()->BrakingFrictionFactor = GateSettingsToApply.BrakingFrictionFactor;
		GetCharacterMovement()->BrakingFriction = GateSettingsToApply.BrakingFriction;
		GetCharacterMovement()->bUseSeparateBrakingFriction = GateSettingsToApply.bUseSeparateBrakingFriction;
	}
}


void ABrandNewPlayerCharacter::OnEquippedWeaponChanged()
{
	// 무기 타입별 애님 레이어를 변경 
	if (WeaponAnimLayerMap.Contains(CurrentEquippedWeaponType))
	{
		GetMesh()->LinkAnimClassLayers(WeaponAnimLayerMap[CurrentEquippedWeaponType]);
	}
}
