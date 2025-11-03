// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewEnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BrainComponent.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "AI/BrandNewAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DataAssets/DataAsset_EnemyAbilities.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/Animation/BnBaseAnimInstanceInterface.h"
#include "Interfaces/UI/BnWidgetInterface.h"
#include "Item/Equipment/BrandNewWeapon.h"
#include "Manager/Pooling/BrandNewObjectPoolManager.h"

ABrandNewEnemyCharacter::ABrandNewEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar Widget Component"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 180.f, 0.f);

	Tags.AddUnique(TEXT("Enemy"));
	
}

void ABrandNewEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = InitialMaxWalkSpeed;

	InitAbilityActorInfo();
	if (HasAuthority())
	{
		GiveAbilitiesToEnemy();
	}

	// 체력바표시
	if (IBnWidgetInterface* WidgetInterface = Cast<IBnWidgetInterface>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		WidgetInterface->SetUIWidgetController(this);
	}
	
	BindAttributeChanged();

}

float ABrandNewEnemyCharacter::GetXPReward() const
{
	return XPReward.GetValueAtLevel(EnemyLevel);
}

float ABrandNewEnemyCharacter::GetInitialMaxWalkSpeed_Implementation() const
{
	return InitialMaxWalkSpeed;
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

void ABrandNewEnemyCharacter::OnCharacterDied_Implementation()
{
	Super::OnCharacterDied_Implementation();

	if (!HasAuthority()) return;

	Multicast_ActivateDisappearEffect();
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic("Pooled");
	}

	// 아이템 드랍
	FGameplayEventData EventData;
	EventData.EventTag = BrandNewGamePlayTag::Event_ItemDrop;
	EventData.Target = this;
	EventData.Instigator = this;
	
	AbilitySystemComponent->HandleGameplayEvent(EventData.EventTag, &EventData);

	// 타이머 설정 후 에너미 Pool로 return
	TWeakObjectPtr WeakThis = this;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (!WeakThis.IsValid()) return;
		
		if (UBrandNewObjectPoolManager* PoolManager = UBrandNewFunctionLibrary::GetObjectPoolManager(WeakThis.Get()))
		{
			WeakThis.Get()->bIsActivated = false;
			if (WeakThis.Get()->CombatWeapon)
			{
				WeakThis.Get()->CombatWeapon->SetWeaponVisible(false);
			}
			
			PoolManager->ReturnObject(WeakThis.Get());
		}
		else
		{
			if (WeakThis.Get()->CombatWeapon)
			{
				WeakThis.Get()->CombatWeapon->Destroy();
			}
			WeakThis->Destroy();
		}

		
	}), DeathWaiteDuration, false);
	
}

bool ABrandNewEnemyCharacter::IsAllocatedToWorld()
{
	return bIsActivated;
}

void ABrandNewEnemyCharacter::Multicast_EnableCapsuleCollision_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

void ABrandNewEnemyCharacter::ActivateEnemy(const FVector& NewLocation, const FRotator& NewRotation)
{
	if (!HasAuthority()) return;
	
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
	
	ApplyEnemyAttribute();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->bPauseAnims = false;
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	Multicast_EnableCapsuleCollision();
	
	GetCharacterMovement()->GravityScale = 1.f;
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	if (CombatWeapon)
	{
		CombatWeapon->SetWeaponVisible(true);
	}

	if (bIsDead && AbilitySystemComponent)
	{
		bIsDead = false;
		FGameplayTagContainer GameplayTags;
		GameplayTags.AddTag(BrandNewGamePlayTag::Ability_Shared_React_Death);
		AbilitySystemComponent->CancelAbilities(&GameplayTags);
	}

	Multicast_ActivateAppearEffect();

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->RestartLogic();
	}
	else
	{
		SpawnDefaultController();
	}
	
	bIsActivated = true;
	
}

void ABrandNewEnemyCharacter::Multicast_ActivateAppearEffect_Implementation()
{
	StartAppearEffect(); // bIsActivated 이거 복제 설정해서 확인해봐야함.
}

void ABrandNewEnemyCharacter::Multicast_ActivateDisappearEffect_Implementation()
{
	StartDissolveEffect();
}


void ABrandNewEnemyCharacter::BindAttributeChanged()
{
	TWeakObjectPtr WeakThis = this;

	// 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (!WeakThis.IsValid()) return;

		WeakThis.Get()->HealthChangedDelegate.Broadcast(Data.NewValue);
		
	});

	// 최대 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (!WeakThis.IsValid()) return;
			
		WeakThis.Get()->MaxHealthChangedDelegate.Broadcast(Data.NewValue);
		
	});

	
	HealthChangedDelegate.Broadcast(AttributeSet->GetHealth());
	MaxHealthChangedDelegate.Broadcast(AttributeSet->GetMaxHealth());
	
	
}

void ABrandNewEnemyCharacter::BindGameplayTagChanged()
{
	AbilitySystemComponent->RegisterGameplayTagEvent(
		BrandNewGamePlayTag::Status_Shared_Strafing, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnStrafingTagChanged);

	AbilitySystemComponent->RegisterGameplayTagEvent(BrandNewGamePlayTag::Status_Shared_SuperArmor).AddUObject(this, &ThisClass::OnSuperArmorTagChanged);
}


void ABrandNewEnemyCharacter::OnStrafingTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (IBnBaseAnimInstanceInterface* AnimInterface = Cast<IBnBaseAnimInstanceInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->SetIsStrafing(NewCount > 0);
	}
}

void ABrandNewEnemyCharacter::OnSuperArmorTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	GameplayTagChangedDelegate.Broadcast(CallbackTag, NewCount > 0);
	
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
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_PhysicalAttackPower, SecondaryDataRow->PhysicalAttackPower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_PhysicalDefensePower, SecondaryDataRow->PhysicalDefensePower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_MagicAttackPower, SecondaryDataRow->MagicAttackPower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_MagicDefensePower, SecondaryDataRow->MagicDefensePower);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_CriticalChance, SecondaryDataRow->CriticalChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, BrandNewGamePlayTag::Attribute_Secondary_CriticalMagnitude, SecondaryDataRow->CriticalMagnitude);
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
			if (!WeakThis.IsValid()) return;
			
			const UDataAsset_EnemyAbilities* LoadedData = WeakThis.Get()->EnemyAbilitiesDataAsset.Get();
			if (IsValid(LoadedData) && IsValid(WeakThis.Get()->AbilitySystemComponent))
			{
				WeakThis.Get()->AbilitySystemComponent->GrantAbilities(LoadedData->ReactAbilities, false);
				WeakThis.Get()->AbilitySystemComponent->GrantAbilities(LoadedData->PassiveAbilities, true);
			}

			if (WeakThis.Get()->CombatWeapon && !WeakThis.Get()->bIsActivated)
			{
				WeakThis.Get()->CombatWeapon->SetWeaponVisible(false);
			}
			
		})
	);
	
}
