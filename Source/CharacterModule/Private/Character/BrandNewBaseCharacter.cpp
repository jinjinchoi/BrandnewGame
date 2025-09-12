// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewBaseCharacter.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interfaces/BnBaseAnimInstanceInterface.h"
#include "Item/Equipment/BrandNewWeapon.h"
#include "Net/UnrealNetwork.h"

ABrandNewBaseCharacter::ABrandNewBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->bReceivesDecals = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UBrandNewAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UBrandNewAttributeSet>("AttributeSet");

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
}

void ABrandNewBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CombatWeapon);
	
}

void ABrandNewBaseCharacter::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (bCanLaunch)
	{
		GetMovementComponent()->StopMovementImmediately();
		Super::LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	}
}


void ABrandNewBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupWeapon();
	
}


void ABrandNewBaseCharacter::InitAbilityActorInfo()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	BindGameplayTagDelegates();
	
}

void ABrandNewBaseCharacter::BindGameplayTagDelegates()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(
		BrandNewGamePlayTag::Status_Shared_Strafing, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnStrafingTagChanged);
}

void ABrandNewBaseCharacter::OnStrafingTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (IBnBaseAnimInstanceInterface* AnimInterface = Cast<IBnBaseAnimInstanceInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->SetIsStrafing(NewCount > 0);
	}
}

void ABrandNewBaseCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const
{
	if (!HasAuthority() || !EffectClass || !AbilitySystemComponent) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}

void ABrandNewBaseCharacter::SetupWeapon()
{
	if (!CombatWeaponClass || !HasAuthority()) return;

	const FVector SocketLocation = GetMesh()->GetSocketLocation(CombatSocketName);
	const FRotator SocketRotation = GetMesh()->GetSocketRotation(CombatSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	CombatWeapon =  GetWorld()->SpawnActor<ABrandNewWeapon>(CombatWeaponClass, SocketLocation, SocketRotation, SpawnParams);
	if (CombatWeapon)
	{
		CombatWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CombatSocketName);
	}
	
	
}



UAbilitySystemComponent* ABrandNewBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABrandNewBaseCharacter::ToggleWeaponCollision_Implementation(bool bEnable)
{
	if (!CombatWeapon) return;

	CombatWeapon->ToggleCollisionEnable(bEnable);
	
}

void ABrandNewBaseCharacter::OnCharacterHit_Implementation(const bool bIsHit)
{
	bIsHitReacting = bIsHit;
}

void ABrandNewBaseCharacter::OnCharacterDied_Implementation()
{
	bIsDead = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

}

bool ABrandNewBaseCharacter::IsHitReacting() const
{
	return bIsHitReacting;
}

UMotionWarpingComponent* ABrandNewBaseCharacter::GetMotionWarpingComponent_Implementation()
{
	return MotionWarpingComponent;
}
