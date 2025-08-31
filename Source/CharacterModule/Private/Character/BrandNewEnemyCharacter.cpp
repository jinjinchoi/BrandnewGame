// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "DataAssets/DataAsset_EnemyAbilities.h"
#include "Engine/AssetManager.h"

ABrandNewEnemyCharacter::ABrandNewEnemyCharacter()
{
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void ABrandNewEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	if (HasAuthority())
	{
		ApplyEnemyAttribute();
		GiveAbilitiesToEnemy();
	}
	
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

void ABrandNewEnemyCharacter::GiveAbilitiesToEnemy()
{
	if (EnemyAbilitiesDataAsset.IsNull() || !HasAuthority() || !AbilitySystemComponent) return;

	TWeakObjectPtr WeakThis = this;

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(
		EnemyAbilitiesDataAsset.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([WeakThis]()
		{
			ABrandNewEnemyCharacter* Enemy = WeakThis.Get();
			if (!IsValid(Enemy)) return;
			
			const UDataAsset_EnemyAbilities* LoadedData = Enemy->EnemyAbilitiesDataAsset.Get();
			if (IsValid(LoadedData) && IsValid(Enemy->AbilitySystemComponent))
			{
				Enemy->AbilitySystemComponent->GrantAbilities(LoadedData->ReactAbilities, false);
				Enemy->AbilitySystemComponent->GrantAbilities(LoadedData->PassiveAbilities, true);
			}
			
		})
	);
	
}
