// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewBaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CharacterFunctionLibrary.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "BrandNewTypes/BrandNewMacro.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
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

	LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox");
	LeftHandCollisionBox->SetupAttachment(GetMesh());
	LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
	RightHandCollisionBox->SetupAttachment(GetMesh());
	RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Ground, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Ground, ECR_Ignore);
	
	
}

#if WITH_EDITOR
void ABrandNewBaseCharacter::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
	{
		LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxAttachBoneName);
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
	{
		RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxAttachBoneName);
	}
	
}
#endif


void ABrandNewBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CombatWeapon);
	
}


void ABrandNewBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupWeapon();

	if (HasAuthority())
	{
		LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
		RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
	}
	
}


void ABrandNewBaseCharacter::InitAbilityActorInfo()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	BindGameplayTagChanged();
	
}

void ABrandNewBaseCharacter::BindGameplayTagChanged()
{

}

void ABrandNewBaseCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const
{
	if (!HasAuthority() || !EffectClass || !AbilitySystemComponent) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}

void ABrandNewBaseCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	const bool bIsNotValid =
	!IsValid(OtherActor) ||
	!OtherActor->Implements<UBrandNewCharacterInterface>() ||
	OverlappedActors.Contains(OtherActor) ||
	!GetOwner() ||
	GetOwner() == OtherActor;

	if (bIsNotValid) return;

	if (!UCharacterFunctionLibrary::IsTargetActorHostile(this, OtherActor))
	{
		return;
	}

	OverlappedActors.Add(OtherActor);

	FGameplayEventData Data;
	Data.Instigator = GetInstigator();
	Data.Target = OtherActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, BrandNewGamePlayTag::Event_Hit_Melee, Data);
}

void ABrandNewBaseCharacter::SetupWeapon()
{
	if (!CombatWeaponClass || !HasAuthority()) return;

	const FVector SocketLocation = GetMesh()->GetSocketLocation(CombatSocketName);
	const FRotator SocketRotation = GetMesh()->GetSocketRotation(CombatSocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	CombatWeapon =  GetWorld()->SpawnActor<ABrandNewWeapon>(CombatWeaponClass, SocketLocation, SocketRotation, SpawnParams);
	if (CombatWeapon)
	{
		CombatWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CombatSocketName);
	}
	
}

void ABrandNewBaseCharacter::ToggleWeaponCollision_Implementation(const bool bEnable)
{
	if (!CombatWeapon) return;

	CombatWeapon->ToggleCollisionEnable(bEnable);
	
}

void ABrandNewBaseCharacter::ToggleCharacterCombatCollision_Implementation(const bool bEnable,
	const ECombatCollisionPosition CollisionPosition)
{
	if (CollisionPosition == ECombatCollisionPosition::None) return;

	if (!bEnable)
	{
		LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
		return;
	}
	
	if (CollisionPosition == ECombatCollisionPosition::LeftHand)
	{
		LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else if (CollisionPosition == ECombatCollisionPosition::RightHand)
	{
		RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ABrandNewBaseCharacter::LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride)
{
	if (bCanLaunch)
	{
		GetMovementComponent()->StopMovementImmediately();
		Super::LaunchCharacter(LaunchVelocity * CharacterLaunchMultiplier, bXYOverride, bZOverride);
	}
}

void ABrandNewBaseCharacter::OnCharacterHit_Implementation(const bool bIsHit)
{
	bIsHitReacting = bIsHit;
}

void ABrandNewBaseCharacter::OnCharacterDied_Implementation()
{
	bIsDead = true;
	OnCharacterDiedDelegate.Broadcast();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GetMovementComponent()->StopMovementImmediately();
	
}

bool ABrandNewBaseCharacter::IsHitReacting() const
{
	return bIsHitReacting;
}

FOnCharacterDied& ABrandNewBaseCharacter::GetOnCharacterDiedDelegate()
{
	return OnCharacterDiedDelegate;
}

UMotionWarpingComponent* ABrandNewBaseCharacter::GetMotionWarpingComponent_Implementation() const
{
	return MotionWarpingComponent;
}

FVector ABrandNewBaseCharacter::GetProjectileSpawnLocation_Implementation(const FName& SocketName) const
{
	return GetMesh()->GetSocketLocation(SocketName);
}

bool ABrandNewBaseCharacter::IsDead_Implementation() const
{
	return bIsDead;
}


UAbilitySystemComponent* ABrandNewBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
