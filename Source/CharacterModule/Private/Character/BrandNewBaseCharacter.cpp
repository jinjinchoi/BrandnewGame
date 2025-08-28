// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewBaseCharacter.h"

#include "CharacterFunctionLibrary.h"
#include "DebugHelper.h"
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
	
	ApplyPrimaryAttribute();
	ApplyGameplayEffectToSelf(SecondaryAttributeEffect, 1.f);
	ApplyGameplayEffectToSelf(VitalAttributeEffect, 1.f);
}

void ABrandNewBaseCharacter::ApplyPrimaryAttribute() const
{
	if (!HasAuthority()) return;
	
	if (!AttributeDataTable || !PrimaryAttributeEffect || !AbilitySystemComponent)
	{
		const FString Msg = FString::Printf(TEXT("Invalid DataTable or GameplayEffect or ASC pointer detected at %s"), *GetName());
		DebugHelper::Print(Msg);
	}
	
	static const FString ContextString(TEXT("Primary Attribute Data"));
	if (const FPrimaryAttributeDataRow* FoundRow = AttributeDataTable->FindRow<FPrimaryAttributeDataRow>(AttributeTableKeyName, ContextString))
	{
		FBaseAttributePrams AttributePrams;
		AttributePrams.Strength = FoundRow->Strength;
		AttributePrams.Dexterity = FoundRow->Dexterity;
		AttributePrams.Intelligence = FoundRow->Intelligence;
		AttributePrams.Vitality = FoundRow->Vitality;
		AttributePrams.MaxHealth = FoundRow->MaxHealth;
		AttributePrams.CurrentHealth = FoundRow->MaxHealth;
		AttributePrams.MaxMana = FoundRow->MaxMana;
		AttributePrams.CurrentMana = FoundRow->MaxMana;
		
		UCharacterFunctionLibrary::ApplyPrimaryAttributesSetByCaller(AttributePrams, AbilitySystemComponent, PrimaryAttributeEffect);
	}
	
}

void ABrandNewBaseCharacter::ApplyGameplayEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClass, const float Level) const
{
	if (!EffectClass || !AbilitySystemComponent) return;

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
