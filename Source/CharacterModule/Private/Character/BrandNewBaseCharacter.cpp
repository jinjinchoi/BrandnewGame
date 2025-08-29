// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewBaseCharacter.h"

#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "BrandNewTypes/BrandNewStructTpyes.h"
#include "Item/Equipment/BrandNewWeapon.h"
#include "Net/UnrealNetwork.h"

ABrandNewBaseCharacter::ABrandNewBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->bReceivesDecals = false;

	AbilitySystemComponent = CreateDefaultSubobject<UBrandNewAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UBrandNewAttributeSet>("AttributeSet");

}

void ABrandNewBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CombatWeapon);
	
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
