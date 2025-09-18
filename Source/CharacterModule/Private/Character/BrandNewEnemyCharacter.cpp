// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "AI/BrandNewAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/DataAsset_EnemyAbilities.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/BnBaseAnimInstanceInterface.h"
#include "Interfaces/BnWidgetInterface.h"

ABrandNewEnemyCharacter::ABrandNewEnemyCharacter()
{
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar Widget Component"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	
}

float ABrandNewEnemyCharacter::GetXPReward() const
{
	return XPReward.GetValueAtLevel(EnemyLevel);
}

void ABrandNewEnemyCharacter::OnCharacterDied_Implementation()
{
	Super::OnCharacterDied_Implementation();

	if (AController* AIController = GetController())
	{
		AIController->UnPossess();
	}
	
}

void ABrandNewEnemyCharacter::OnCharacterHit_Implementation(const bool bIsHit)
{
	Super::OnCharacterHit_Implementation(bIsHit);

	if (ABrandNewAIController* AIController = Cast<ABrandNewAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool("IsHit", bIsHit);
		}
	}
	
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

	if (IBnWidgetInterface* WidgetInterface = Cast<IBnWidgetInterface>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		WidgetInterface->SetUIWidgetController(this);
	}

	BindAttributeChanged();

}

void ABrandNewEnemyCharacter::BindAttributeChanged()
{
	TWeakObjectPtr WeakThis = this;

	// 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		// const FString Msg = FString::Printf(TEXT("Current Health: %f"), Data.NewValue);
		// DebugHelper::Print(WeakThis.Get(), Msg, FColor::Yellow);
		if (const ABrandNewEnemyCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->HealthChangedDelegate.Broadcast(Data.NewValue);
		}
	});

	// 최대 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewEnemyCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->MaxHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	});

	HealthChangedDelegate.Broadcast(AttributeSet->GetHealth());
	MaxHealthChangedDelegate.Broadcast(AttributeSet->GetMaxHealth());
	
}

void ABrandNewEnemyCharacter::BindGameplayTagDelegates()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(
		BrandNewGamePlayTag::Status_Shared_Strafing, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnStrafingTagChanged);
}

void ABrandNewEnemyCharacter::OnStrafingTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (IBnBaseAnimInstanceInterface* AnimInterface = Cast<IBnBaseAnimInstanceInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->SetIsStrafing(NewCount > 0);
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
