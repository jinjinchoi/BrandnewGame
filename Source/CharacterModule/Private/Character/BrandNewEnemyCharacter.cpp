// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"

ABrandNewEnemyCharacter::ABrandNewEnemyCharacter()
{
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void ABrandNewEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	
}

void ABrandNewEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	ApplyEnemyAttribute();
}

FSecondaryAttributeDataRow* ABrandNewEnemyCharacter::FindEnemyDataRow() const
{
	if (!SecondaryAttributeDataTable) return nullptr;

	const FName RowName = *FString::Printf(TEXT("%s_Lv%d"), *EnemyName.ToString(), EnemyLevel);
	return SecondaryAttributeDataTable->FindRow<FSecondaryAttributeDataRow>(RowName, TEXT("EnemyData Lookup"));
	
}

void ABrandNewEnemyCharacter::ApplyEnemyAttribute() const
{
	if (!HasAuthority() || !SecondaryAttributeEffect || !AbilitySystemComponent) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FSecondaryAttributeDataRow* SecondaryDataRow = FindEnemyDataRow();
	if (!SecondaryDataRow) return;
	
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(SecondaryAttributeEffect, 1.f, ContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondry_PhysicalAttackPower, SecondaryDataRow->PhysicalAttackPower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondry_PhysicalDefensePower, SecondaryDataRow->PhysicalDefensePower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondry_MagicAttackPower, SecondaryDataRow->MagicAttackPower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondry_MagicDefensePower, SecondaryDataRow->MagicDefensePower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondry_CriticalChance, SecondaryDataRow->CriticalChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondry_CriticalMagnitude, SecondaryDataRow->CriticalMagnitude);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_MaxHealth, SecondaryDataRow->MaxHealth);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Vital_CurrentHealth, SecondaryDataRow->MaxHealth);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

}
