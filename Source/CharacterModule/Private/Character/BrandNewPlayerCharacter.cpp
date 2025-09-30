// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CharacterFunctionLibrary.h"
#include "DebugHelper.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/DataAsset_AttributeInfo.h"
#include "DataAssets/DataAsset_DefaultPlayerAbilities.h"
#include "DataAssets/DataAsset_LevelUpInfo.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Player/BnPlayerControllerInterface.h"
#include "Interfaces/UI/BrandNewHUDInterface.h"
#include "Interfaces/Animation/BrandNewPlayerAnimInterface.h"
#include "Item/Equipment/BrandNewWeapon.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/HUD.h"
#include "AbilitySystem/Abilities/BandNewBaseGameplayAbility.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "Game/GameInstance/BrandNewGameInstance.h"
#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "Interfaces/Animation/BnBaseAnimInstanceInterface.h"
#include "Inventory/BrandNewInventory.h"
#include "PickupItems/BrandNewPickupItem.h"
#include "Player/BrandNewPlayerState.h"


ABrandNewPlayerCharacter::ABrandNewPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
	
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	Tags.AddUnique(TEXT("Player"));
	
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
	}
}

void ABrandNewPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGate);
	DOREPLIFETIME(ThisClass, EquippedWeaponType);
	
}

void ABrandNewPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Server_RequestUpdateMovementMode(CurrentGate);
	InitAbilityActorInfo();
	InitializePrimaryAttribute();
	BindAttributeDelegates();
	AddCharacterAbilities(); // TODO: 어빌리티 레벨 로드 해야함.
}

void ABrandNewPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdateMovementComponentPrams();
	InitAbilityActorInfo(); // ASC를 초기화 하고 기본 능력치 적용
	BindAttributeDelegates(); // 그후 Attribute 변화를 바인딩(위젯에 알리기 위한 용도)
	InitHUDAndBroadCastInitialValue(); // 바인딩이 끝났으면 HUD 초기화 요청, HUD에서는 위젯 구성하고 위젯에서 초기값을 요청함.
}


void ABrandNewPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnEquippedWeaponChanged();
}

void ABrandNewPlayerCharacter::InitializePrimaryAttribute() const
{
	if (!HasAuthority()) return;

	// 세이브 데이터 유무에 따라 세이브 데이터 또는 데이터 테이블에서 값을 가져와 Primary Attribute 적용
	const FSaveSlotPrams SavedData = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>()->GetLastestPlayerData();
	if (SavedData.bIsValid)
	{
		ApplyPrimaryAttributeFromSaveData(SavedData.AttributePrams);
		ApplyGameplayEffectToSelf(SecondaryAttributeEffect, 1.f);
		OverrideVitalAttribute(SavedData.AttributePrams.CurrentHealth, SavedData.AttributePrams.CurrentMana);
	}
	else
	{
		ApplyPrimaryAttributeFromDataTable();
		ApplyGameplayEffectToSelf(SecondaryAttributeEffect, 1.f);
		ApplyGameplayEffectToSelf(VitalAttributeEffect, 1.f);
	}
}

void ABrandNewPlayerCharacter::ApplyPrimaryAttributeFromSaveData(const FAttributeSaveData& SlotPrams) const
{
	if (!HasAuthority() || !AbilitySystemComponent || !PrimaryAttributeEffect) return;

	FBaseAttributePrams AttributePrams;
	AttributePrams.Strength = SlotPrams.Strength;
	AttributePrams.Dexterity = SlotPrams.Dexterity;
	AttributePrams.Intelligence = SlotPrams.Intelligence;
	AttributePrams.Vitality = SlotPrams.Vitality;
	AttributePrams.Level = SlotPrams.Level;
	AttributePrams.AttributePoint = SlotPrams.AttributePoint;
	AttributePrams.XP = SlotPrams.Experience;
	
	UCharacterFunctionLibrary::ApplyPrimaryAttributesSetByCaller(AttributePrams, AbilitySystemComponent, PrimaryAttributeEffect);
}

void ABrandNewPlayerCharacter::ApplyPrimaryAttributeFromDataTable() const
{
	if (!HasAuthority()) return;
	
	if (!AttributeDataTable || !PrimaryAttributeEffect || !AbilitySystemComponent)
	{
		const FString Msg = FString::Printf(TEXT("Invalid DataTable or GameplayEffect or ASC pointer detected at %s"), *GetName());
		DebugHelper::Print(Msg);
		return;
	}
	
	static const FString ContextString(TEXT("Primary Attribute Data"));
	if (const FPrimaryAttributeDataRow* FoundRow = AttributeDataTable->FindRow<FPrimaryAttributeDataRow>(AttributeTableKeyName, ContextString))
	{
		FBaseAttributePrams AttributePrams;
		AttributePrams.Strength = FoundRow->Strength;
		AttributePrams.Dexterity = FoundRow->Dexterity;
		AttributePrams.Intelligence = FoundRow->Intelligence;
		AttributePrams.Vitality = FoundRow->Vitality;
		AttributePrams.Level = 1.f;
		
		UCharacterFunctionLibrary::ApplyPrimaryAttributesSetByCaller(AttributePrams, AbilitySystemComponent, PrimaryAttributeEffect);
	}
}

void ABrandNewPlayerCharacter::OverrideVitalAttribute(const float HealthToApply, const float ManaToApply) const
{
	check(VitalOverrideEffect);
	if (!HasAuthority() || !VitalOverrideEffect || !AbilitySystemComponent) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(VitalOverrideEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Vital_CurrentHealth, HealthToApply);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Vital_CurrentMana, ManaToApply);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ABrandNewPlayerCharacter::ApplyAddXPEffect(const float XpToAdd) const
{
	check(XPAttributeEffect);
	if (!HasAuthority() || !XPAttributeEffect || !AbilitySystemComponent) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(XPAttributeEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Experience_XP, XpToAdd);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}

void ABrandNewPlayerCharacter::ApplyLevelUpGameplayEffect(const int32 LevelToApply, const int32 RewardAttributePoint)
{
	check(LevelUpEffect);
	if (!HasAuthority() || !LevelUpEffect || !AbilitySystemComponent) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(LevelUpEffect, 1.f, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Experience_Level, LevelToApply);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Experience_AttributePoint, RewardAttributePoint);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}

int32 ABrandNewPlayerCharacter::FindLevelForXP(const int32 InXP) const
{
	if (!LevelUpInfoDataAsset) return 0;

	return LevelUpInfoDataAsset->FindLevelForXP(InXP);
	
}

int32 ABrandNewPlayerCharacter::GetAttributePointsReward(const int32 LevelToFind) const
{
	if (!LevelUpInfoDataAsset) return 0;
	
	return LevelUpInfoDataAsset->LevelUpInformation[LevelToFind].AttributePointAward;
}

void ABrandNewPlayerCharacter::UpgradeAttribute(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams)
{
	check(AttributeUpgradeEffect)
	if (!AttributeUpgradeEffect || !AbilitySystemComponent) return;

	Server_RequestUpgradeAttribute(AttributeUpgradePrams);
	
}

void ABrandNewPlayerCharacter::Server_RequestUpgradeAttribute_Implementation(const TArray<FAttributeUpgradePrams>& AttributeUpgradePrams)
{
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributeUpgradeEffect, 1.f, ContextHandle);

	int32 ConsumedStatPoint = 0;
	
	for (const FAttributeUpgradePrams& UpgradePrams : AttributeUpgradePrams)
	{
		if (!UpgradePrams.TagToUpgrade.IsValid()) continue;

		ConsumedStatPoint += UpgradePrams.UpgradeAmount;
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,UpgradePrams.TagToUpgrade, UpgradePrams.UpgradeAmount);
	}

	if (GetAttributeValueByTag(BrandNewGamePlayTag::Attribute_Experience_AttributePoint) < ConsumedStatPoint) return;
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Experience_AttributePoint, -ConsumedStatPoint);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}




void ABrandNewPlayerCharacter::AddCharacterAbilities() const
{
	if (!HasAuthority() || DefaultAbilitiesDataAsset.IsNull() || !AbilitySystemComponent) return;

	TWeakObjectPtr WeakThis = this;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(
		DefaultAbilitiesDataAsset.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([WeakThis]()
		{
			const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get();
			if (!IsValid(PlayerCharacter))  return;
			
			const UDataAsset_DefaultPlayerAbilities* LoadedData = PlayerCharacter->DefaultAbilitiesDataAsset.Get();
			if (IsValid(LoadedData) && IsValid(PlayerCharacter->AbilitySystemComponent))
			{
				PlayerCharacter->AbilitySystemComponent->GrantAbilities(LoadedData->PassiveAbilities, true);
				PlayerCharacter->AbilitySystemComponent->GrantAbilities(LoadedData->ReactAbilities, false);
				PlayerCharacter->AbilitySystemComponent->GrantPlayerInputAbilities(LoadedData->InputAbilities);
			}
			
			PlayerCharacter->InitHUDAndBroadCastInitialValue();
			
		})
	);
}



void ABrandNewPlayerCharacter::BindAttributeDelegates()
{
	if (!IsLocallyControlled() || !AbilitySystemComponent || !AttributeSet) return;

	TWeakObjectPtr WeakThis = this;

	// 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		// const FString Msg = FString::Printf(TEXT("Current Health: %f"), Data.NewValue);
		// DebugHelper::Print(WeakThis.Get(), Msg, FColor::Yellow);
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->HealthChangedDelegate.ExecuteIfBound(Data.NewValue);
		}
	});

	// 최대 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->MaxHealthChangedDelegate.ExecuteIfBound(Data.NewValue);
		}
	});

	// 마나 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetManaAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->ManaChangedDelegate.ExecuteIfBound(Data.NewValue);
		}
	});

	// 최대 마나 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxManaAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->MaxManaChangedDelegate.ExecuteIfBound(Data.NewValue);
		}
	});
	
}

void ABrandNewPlayerCharacter::InitHUDAndBroadCastInitialValue() const
{
	if (!IsLocallyControlled()) return;
	
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	IBrandNewHUDInterface* BrandNewHUD = Cast<IBrandNewHUDInterface>(PlayerController->GetHUD());
	if (!BrandNewHUD) return;
	BrandNewHUD->RequestInitHUD();
	
}


void ABrandNewPlayerCharacter::OnAbilityInputPressed(const FGameplayTag& InInputTag) const
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
	
}

void ABrandNewPlayerCharacter::OnAbilityInputReleased(const FGameplayTag& InInputTag) const
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}



AActor* ABrandNewPlayerCharacter::GetCombatWeaponActor_Implementation() const
{
	return CombatWeapon;
}


ECombatWeaponType ABrandNewPlayerCharacter::GetCurrentEquippedWeaponType() const
{
	return EquippedWeaponType;
}


void ABrandNewPlayerCharacter::OnWeaponEquipped_Implementation()
{
	if (!HasAuthority()) return;
	
	EquippedWeaponType = CombatWeapon->GetCombatWeaponType();
	OnEquippedWeaponChanged();
}

void ABrandNewPlayerCharacter::OnWeaponUnequipped_Implementation()
{
	if (!HasAuthority()) return;
	
	EquippedWeaponType = ECombatWeaponType::Unequipped;
	OnEquippedWeaponChanged();
}

void ABrandNewPlayerCharacter::SetStrafeState_Implementation()
{
	const bool bIsStrafing =
		UCharacterFunctionLibrary::DoseActorHasTag(this, BrandNewGamePlayTag::Status_Player_Fire) ||
		UCharacterFunctionLibrary::DoseActorHasTag(this, BrandNewGamePlayTag::Status_Player_LockOn);

	if (IBnBaseAnimInstanceInterface* BaseAnimInterface = Cast<IBnBaseAnimInstanceInterface>(GetMesh()->GetAnimInstance()))
	{
		BaseAnimInterface->SetIsStrafing(bIsStrafing);
	}
}

void ABrandNewPlayerCharacter::SetFireMode_Implementation(const bool IsFiring)
{
	if (IsFiring)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		LastGate = CurrentGate;
		SetMovementMode(EGate::Walking);
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		SetMovementMode(LastGate);
	}
	
}

void ABrandNewPlayerCharacter::SetCombatTargetActor_Implementation(AActor* TargetActor)
{
	if (TargetActor)
	{
		CombatTargetActor = TargetActor;
	}
	else
	{
		CombatTargetActor.Reset();
	}
}

AActor* ABrandNewPlayerCharacter::GetCombatTargetActor_Implementation()
{
	if (CombatTargetActor.IsValid())
	{
		return CombatTargetActor.Get();
	}

	return nullptr;
	
}


void ABrandNewPlayerCharacter::OnRep_CurrentEquippedWeaponType()
{
	OnEquippedWeaponChanged();
}

FText ABrandNewPlayerCharacter::GetCurrentTimeText() const
{
	const FDateTime Now = FDateTime::Now();

	const FString FormattedTime = FString::Printf(TEXT("%d년 %02d월 %02d일 %02d시 %02d분 %02d초"),
		Now.GetYear(),
		Now.GetMonth(),
		Now.GetDay(),
		Now.GetHour(),
		Now.GetMinute(),
		Now.GetSecond());

	return FText::FromString(FormattedTime);
}


void ABrandNewPlayerCharacter::OnEquippedWeaponChanged()
{
	// 무기 타입별 애님 레이어를 변경 
	if (WeaponAnimLayerMap.Contains(EquippedWeaponType))
	{
		GetMesh()->LinkAnimClassLayers(WeaponAnimLayerMap[EquippedWeaponType]);
	}

	// 인풋 매핑 컨텍스트를 변경. 매핑 컨텍스트는 로컬 컨트롤러에서만 바꾸는 것이 의미 있음. (이 함수는 서버 클라이언트 모두에게 실행됨)
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->IsLocalController())
	{
		IBnPlayerControllerInterface* PlayerControllerInterface = Cast<IBnPlayerControllerInterface>(PC);
		if (!PlayerControllerInterface) return;

		if (LastEquippedWeaponType > ECombatWeaponType::Unequipped)
		{
			PlayerControllerInterface->RemoveInputMappingForWeapon(LastEquippedWeaponType);	
		}

		if (EquippedWeaponType > ECombatWeaponType::Unequipped)
		{
			PlayerControllerInterface->AddInputMappingForWeapon(EquippedWeaponType);
		}

		WeaponChangedDelegate.ExecuteIfBound(EquippedWeaponType);
		
	}

	LastEquippedWeaponType = EquippedWeaponType;
	
}

float ABrandNewPlayerCharacter::GetAttributeValueByTag(const FGameplayTag& AttributeTag) const
{
	check(AttributeInfoDataAsset)
	if (!AttributeInfoDataAsset || !AttributeSet) return 0.0f;

	const FGameplayAttribute AttributeGetter = AttributeInfoDataAsset->FindAttributeGetter(AttributeTag);
	
	return AttributeGetter.GetNumericValue(AttributeSet);
	
}

void ABrandNewPlayerCharacter::RequestBroadCastAttributeValue()
{
	MaxHealthChangedDelegate.ExecuteIfBound(AttributeSet->GetMaxHealth());
	HealthChangedDelegate.ExecuteIfBound(AttributeSet->GetHealth());
	MaxManaChangedDelegate.ExecuteIfBound(AttributeSet->GetMaxMana());
	ManaChangedDelegate.ExecuteIfBound(AttributeSet->GetMana());
}

FOnAttributeChangedDelegate& ABrandNewPlayerCharacter::GetHealthChangedDelegate()
{
	return HealthChangedDelegate;
}

FOnAttributeChangedDelegate& ABrandNewPlayerCharacter::GetMaxHealthChangedDelegate()
{
	return MaxHealthChangedDelegate;
}

FOnAttributeChangedDelegate& ABrandNewPlayerCharacter::GetManaChangedDelegate()
{
	return ManaChangedDelegate;
}

FOnAttributeChangedDelegate& ABrandNewPlayerCharacter::GetMaxManaChangedDelegate()
{
	return MaxManaChangedDelegate;
}

FOnWeaponChangedDelegate& ABrandNewPlayerCharacter::GetWeaponChangedDelegate()
{
	return WeaponChangedDelegate;
}

FOnOverlappedItemChangedDelegate& ABrandNewPlayerCharacter::GetOnOverlapChangedDelegate()
{
	return OnOverlappedItemChangedDelegate;
}

float ABrandNewPlayerCharacter::GetRequiredAbilityMana(const FGameplayTag& AbilityTag) const
{
	if (!AbilitySystemComponent) return 0;
	
	const FGameplayAbilitySpecHandle FoundSpecHandle = AbilitySystemComponent->FindAbilitySpecHandleFromAbilityTag(AbilityTag);
	if (!FoundSpecHandle.IsValid()) return 0;
	
	const FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(FoundSpecHandle);
	if (!AbilitySpec) return 0.f;
	
	if (const UBandNewBaseGameplayAbility* Ability = Cast<UBandNewBaseGameplayAbility>(AbilitySpec->Ability))
	{
		return Ability->GetManaCost();
	}
	

	return 0;
	
}


void ABrandNewPlayerCharacter::RequestSave(const FString& SlotName, const int32 SlotIndex)
{
	if (!AbilitySystemComponent || !AttributeSet) return;

	// Attribute를 저장하는 구조체 생성
	FAttributeSaveData AttributeParams;
	AttributeParams.Strength = AttributeSet->GetStrength();
	AttributeParams.Intelligence = AttributeSet->GetIntelligence();
	AttributeParams.Dexterity = AttributeSet->GetDexterity();
	AttributeParams.Vitality = AttributeSet->GetVitality();
	AttributeParams.CurrentHealth = AttributeSet->GetHealth();
	AttributeParams.CurrentMana = AttributeSet->GetMana();
	AttributeParams.Level = AttributeSet->GetCharacterLevel();
	AttributeParams.Experience = AttributeSet->GetXP();
	AttributeParams.AttributePoint = AttributeSet->GetAttributePoint();

	// 세이브 데이터를 저장하는 구조체 생성
	FSaveSlotPrams SaveSlotPrams;
	SaveSlotPrams.AttributePrams = AttributeParams; // Attribute 저장
	SaveSlotPrams.CharacterLocation = GetActorLocation();
	SaveSlotPrams.SavedTime = GetCurrentTimeText();
	SaveSlotPrams.AbilityMap = AbilitySystemComponent->GetAbilityTagLevelMap();
	SaveSlotPrams.TitleText =  FText::FromString(TEXT("퀘스트 구현시 만들어줘야 함"));

	const FString MapName = UWorld::RemovePIEPrefix(GetWorld()->GetOutermost()->GetName());
	SaveSlotPrams.MapPackageName = MapName;
	SaveSlotPrams.MapName = GetGameInstance<UBrandNewGameInstance>()->GetMapName(MapName);
	
	SaveSlotPrams.bIsValid = true;

	UBrandNewSaveSubsystem::SaveGameToSlot(SlotName, SlotIndex, SaveSlotPrams);
	
}

void ABrandNewPlayerCharacter::AddOverlappedItem(AActor* OverlappedItem)
{
	if (HasAuthority())
	{
		OverlappedItems.Add(OverlappedItem);
	}
	else
	{
		OverlappedItemsForUI.Add(OverlappedItem);
	}
	SendPickupInfoToUi(OverlappedItem, true);
	
}

void ABrandNewPlayerCharacter::RemoveOverlappedItem(AActor* OverlappedItem)
{
	if (HasAuthority())
	{
		OverlappedItems.RemoveSingle(OverlappedItem);
	}
	else
	{
		OverlappedItemsForUI.RemoveSingle(OverlappedItem);
	}
	SendPickupInfoToUi(OverlappedItem, false);
	
}

void ABrandNewPlayerCharacter::SendPickupInfoToUi(AActor* ItemToSend, const bool bIsBeginOverlap) const
{
	if (!IsLocallyControlled()) return;
	
	const IPickupItemInterface* PickupItemInterface = CastChecked<IPickupItemInterface>(ItemToSend);
	FPickupsUiInfo PickupUiInfo;
	PickupUiInfo.ItemId = PickupItemInterface->GetId();
	PickupUiInfo.Quantity = PickupItemInterface->GetQuantity();
	PickupUiInfo.UniqueId = PickupItemInterface->GetUniqueId();

	OnOverlappedItemChangedDelegate.ExecuteIfBound(bIsBeginOverlap, PickupUiInfo);
}



void ABrandNewPlayerCharacter::InteractIfPossible()
{

	if (HasAuthority())
	{
		if (!OverlappedItems.IsEmpty())
		{
			for (const TWeakObjectPtr<AActor>& Item : OverlappedItems)
			{
				if (Item.IsValid())
				{
					SendPickupInfoToUi(Item.Get(), false);
				}
			}
			AcquireItem();
		}
	}
	else
	{
		if (!OverlappedItemsForUI.IsEmpty())
		{
			for (const TWeakObjectPtr<AActor>& Item : OverlappedItemsForUI)
			{
				if (Item.IsValid())
				{
					SendPickupInfoToUi(Item.Get(), false);
				}
			}
			OverlappedItemsForUI.Empty();
			Server_AcquireItem();
		}
	}
	
}

void ABrandNewPlayerCharacter::Server_AcquireItem_Implementation()
{
	AcquireItem();
}

void ABrandNewPlayerCharacter::AcquireItem()
{
	const ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	check(BrandNewPlayerState);
	UBrandNewInventory* Inventory = BrandNewPlayerState->GetInventoryInterfaceClass();
	check(Inventory);
	
	for (int32 i = OverlappedItems.Num() - 1; i >= 0; --i)
	{
		if (!OverlappedItems[i].IsValid()) continue;

		ABrandNewPickupItem* BrandNewItem = Cast<ABrandNewPickupItem>(OverlappedItems[i].Get());
		if (!BrandNewItem) continue;
		
		FInventorySlotData ItemData;
		ItemData.ItemID = BrandNewItem->GetId();
		ItemData.Quantity = BrandNewItem->GetQuantity();

		Inventory->AddItemToSlot(ItemData);

		OverlappedItems.RemoveAt(i);
		BrandNewItem->Destroy();
	}
	
	
}

#pragma region Movement
void ABrandNewPlayerCharacter::Server_RequestUpdateMovementMode_Implementation(const EGate NewGate)
{
	SetMovementMode(NewGate);
}

void ABrandNewPlayerCharacter::SetMovementMode(const EGate NewGate)
{
	CurrentGate = (CurrentGate == NewGate) ? EGate::Jogging : NewGate;
	
	UpdateMovementComponentPrams();
	
}

void ABrandNewPlayerCharacter::OnRep_CurrentGate()
{
	UpdateMovementComponentPrams();
	
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

	if (IBrandNewPlayerAnimInterface* AnimInterface = Cast<IBrandNewPlayerAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->UpdateCurrentGate(CurrentGate);
	}
	
}



void ABrandNewPlayerCharacter::AddYawRotation(const float DeltaYaw)
{
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw += DeltaYaw;
	SetActorRotation(NewRot);
}
#pragma endregion
