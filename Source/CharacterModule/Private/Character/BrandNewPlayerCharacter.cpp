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
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Player/BnPlayerControllerInterface.h"
#include "Interfaces/UI/BrandNewHUDInterface.h"
#include "Interfaces/Animation/BrandNewPlayerAnimInterface.h"
#include "Item/Equipment/BrandNewWeapon.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/HUD.h"
#include "AbilitySystem/Abilities/BandNewBaseGameplayAbility.h"
#include "Components/BrandnewQuestComponent.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "Game/GameInstance/BrandNewGameInstance.h"
#include "Game/GameState/BrandNewGameState.h"
#include "Game/Subsystem/BrandNewSaveSubsystem.h"
#include "GameMode/BrandNewGameModeBase.h"
#include "Interfaces/Actor/BrandNewNPCInterface.h"
#include "Interfaces/Actor/QuestActorInterface.h"
#include "Inventory/BrandNewInventory.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/Sequnce/SequenceManager.h"
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
	
}


void ABrandNewPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnEquippedWeaponChanged();

	// 처음 시작할때 위치를 저장하여 맵을 이동하더라도 이동 된 월드의 초기 Location을 알 수 있게 함.
	SafeLocation = GetActorLocation();
	
}


void ABrandNewPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetMovementMode(CurrentGate);
	InitAbilityActorInfo();
	AddCharacterAbilities(); // TODO: 어빌리티 레벨 구현시 로드해야함
	
	if (IsLocallyControlled())
	{
		const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
		check(SaveSubsystem);
		Server_RequestInitCharacterInfo(SaveSubsystem->GetUniqueIdentifier());
		
		BindAttributeDelegates();
		InitHUDAndBroadCastInitialValue();
	}
	
}

void ABrandNewPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	UpdateMovementComponentPrams();
	InitAbilityActorInfo(); // ASC를 초기화

	if (IsLocallyControlled())
	{
		BindAttributeDelegates(); // Attribute 변화를 바인딩(위젯에 알리기 위한 용도)
		
		const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
		check(SaveSubsystem);
		Server_RequestInitCharacterInfo(SaveSubsystem->GetUniqueIdentifier());
	}
	
}


void ABrandNewPlayerCharacter::Server_RequestInitCharacterInfo_Implementation(const FString& PlayerId)
{
	const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	GetPlayerStateChecked<ABrandNewPlayerState>()->PlayerUniqueId = PlayerId;
	
	if (const FSaveSlotPrams LatestPlayerData = SaveSubsystem->GetLatestPlayerData(PlayerId); LatestPlayerData.bIsValid) // 맵 이동인지 접속인지 확인
	{
		// 맵 이동일 경우 SaveSubsystem에 있는 LatestPlayerData로 복구
		LoadCharacterData(LatestPlayerData);
	}
	else
	{
		// 접속인 경우 로드인지 뉴게임인지 확인하고 데이터 복구
		InitializeCharacterInfo(PlayerId);
	}
	
	GetWorld()->GetGameState<ABrandNewGameState>()->RegisterPlayerState(GetPlayerState());
	Client_OnCharacterInfoSet();
}


void ABrandNewPlayerCharacter::Client_OnCharacterInfoSet_Implementation()
{
	if (IsLocallyControlled())
	{
		InitHUDAndBroadCastInitialValue(); // 바인딩이 끝났으면 HUD 초기화 요청, HUD에서는 위젯 구성하고 위젯에서 초기값을 요청함.
	}
}

void ABrandNewPlayerCharacter::InitializeCharacterInfo(const FString& UniqueId)
{
	if (!HasAuthority()) return;

	const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;

	// 로드된 맵이며 동시에 내 세이브 파일이 존재하면 로드 진행
	if (SaveSubsystem->IsLoadedWorld() && SaveSubsystem->GetCurrentSlotSaveDataById(UniqueId).bIsValid)
	{
		const FSaveSlotPrams SavedData = SaveSubsystem->GetCurrentSlotSaveDataById(UniqueId);
		LoadCharacterData(SavedData);
		return;
	}
	
	// NewGame으로 시작한 경우 Default 데이터 설정.
	ApplyPrimaryAttributeFromDataTable();
	ApplyGameplayEffectToSelf(SecondaryAttributeEffect, 1.f);
	ApplyGameplayEffectToSelf(VitalAttributeEffect, 1.f);
	ApplyGameplayEffectToSelf(RegenerationEffect, 1.f);
	
	// TODO: 퀘스트 세이브 로드 구현해야함
	GetPlayerStateChecked<ABrandNewPlayerState>()->GrantQuestByLevelRequirement(1);

	if (IsLocallyControlled())
	{
		// 완전 처음 게임을 시작. 시네마틱 재생
		PlayFirstEntranceSequence();
	}
	else
	{
		Client_PlayFirstEntranceSequence();
	}
	
}

void ABrandNewPlayerCharacter::PlayFirstEntranceSequence() const
{
	USequenceManager* SequenceManager = GetGameInstance<UBrandNewGameInstance>()->GetSequenceManager();
	if (!SequenceManager) return;

	SequenceManager->PlayFirstEntranceSequence();
	
}

void ABrandNewPlayerCharacter::Client_PlayFirstEntranceSequence_Implementation()
{
	PlayFirstEntranceSequence();
}


void ABrandNewPlayerCharacter::LoadCharacterData(const FSaveSlotPrams& SavedDataToApply)
{
	if (!SavedDataToApply.bIsValid) return;
	
	ApplyPrimaryAttributeFromSaveData(SavedDataToApply.AttributePrams);
	ApplyGameplayEffectToSelf(SecondaryAttributeEffect, 1.f);
	ApplyGameplayEffectToSelf(RegenerationEffect, 1.f);
	LoadInventory(SavedDataToApply.InventoryContents); // 인벤토리 로드
	OverrideVitalAttribute(SavedDataToApply.AttributePrams.CurrentHealth, SavedDataToApply.AttributePrams.CurrentMana);

	const FString MapName = UWorld::RemovePIEPrefix(GetWorld()->GetOutermost()->GetName());
	if (SavedDataToApply.MapPackageName == MapName)
	{
		MoveCharacterToValidLocation(SavedDataToApply.CharacterLocation);
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



void ABrandNewPlayerCharacter::LoadInventory(const FInventoryContents& InventoryData)
{
	const ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	if (!BrandNewPlayerState) return;
	UBrandNewInventory* Inventory = BrandNewPlayerState->GetInventory();
	if (!Inventory) return;
	
	Inventory->SetInventoryContents(InventoryData);
	
	if (Inventory->GetLastEquippedWeaponSlotIndex() != INDEX_NONE)
	{
		EquipItem(Inventory->GetLastEquippedWeaponSlotIndex(), EItemType::Weapon);
	}
	if (Inventory->GetLastEquippedArmorSlotIndex() != INDEX_NONE)
	{
		EquipItem(Inventory->GetLastEquippedArmorSlotIndex(), EItemType::Armor);
	}
		
}

void ABrandNewPlayerCharacter::ApplyAddXPEffect(const float XpToAdd) const
{
	check(XPAttributeEffect);
	if (!HasAuthority() || !XPAttributeEffect || !AbilitySystemComponent || XpToAdd == 0) return;

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

	FGameplayCueParameters CueParameters;
	CueParameters.SourceObject = this;
	CueParameters.EffectCauser = this;
	CueParameters.EffectContext = ContextHandle;
	CueParameters.Location = GetActorLocation();
	
	AbilitySystemComponent->ExecuteGameplayCue(BrandNewGamePlayTag::GameplayCue_Effect_Levelup, CueParameters);
	
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
	if (!HasAuthority() || !IsValid(DefaultAbilitiesDataAsset) || !AbilitySystemComponent) return;

	AbilitySystemComponent->GrantAbilities(DefaultAbilitiesDataAsset->PassiveAbilities, true);
	AbilitySystemComponent->GrantAbilities(DefaultAbilitiesDataAsset->PassiveAbilities, true);
	AbilitySystemComponent->GrantAbilities(DefaultAbilitiesDataAsset->ReactAbilities, false);
	AbilitySystemComponent->GrantPlayerInputAbilities(DefaultAbilitiesDataAsset->InputAbilities);
}


void ABrandNewPlayerCharacter::BindAttributeDelegates()
{
	if (!IsLocallyControlled() || !AbilitySystemComponent || !AttributeSet) return;

	TWeakObjectPtr WeakThis = this;

	// 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->HealthChangedDelegate.Broadcast(Data.NewValue);
		}
	});

	// 최대 체력 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->MaxHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	});

	// 마나 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetManaAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->ManaChangedDelegate.Broadcast(Data.NewValue);
		}
	});

	// 최대 마나 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxManaAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->MaxManaChangedDelegate.Broadcast(Data.NewValue);
		}
	});

	// 레벨 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetCharacterLevelAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->LevelChangedDelegate.Broadcast(Data.NewValue);
			PlayerCharacter->ExperienceChangedDelegate.Broadcast(PlayerCharacter->CalculateExpPercent());
		}
	});

	// 경험치 변경 바인딩
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetXPAttribute()).AddLambda([WeakThis](const FOnAttributeChangeData& Data)
	{
		if (const ABrandNewPlayerCharacter* PlayerCharacter = WeakThis.Get())
		{
			PlayerCharacter->ExperienceChangedDelegate.Broadcast(PlayerCharacter->CalculateExpPercent());
		}
	});
	
}

void ABrandNewPlayerCharacter::RequestBroadCastAttributeValue()
{
	MaxHealthChangedDelegate.Broadcast(AttributeSet->GetMaxHealth());
	HealthChangedDelegate.Broadcast(AttributeSet->GetHealth());
	MaxManaChangedDelegate.Broadcast(AttributeSet->GetMaxMana());
	ManaChangedDelegate.Broadcast(AttributeSet->GetMana());
	LevelChangedDelegate.Broadcast(AttributeSet->GetCharacterLevel());
	ExperienceChangedDelegate.Broadcast(CalculateExpPercent());
	
}


float ABrandNewPlayerCharacter::CalculateExpPercent() const
{
	if (!AttributeSet || !LevelUpInfoDataAsset) return 0.f;
	
	const int32 PlayerLevel = AttributeSet->GetCharacterLevel();
	if (PlayerLevel < 1) return 0.f;
			
	if (LevelUpInfoDataAsset->LevelUpInformation.IsValidIndex(PlayerLevel) && LevelUpInfoDataAsset->LevelUpInformation.IsValidIndex(PlayerLevel + 1))
	{
		const FLevelUpInfo& NextLevelUpInfo = LevelUpInfoDataAsset->LevelUpInformation[PlayerLevel + 1];
		const FLevelUpInfo& CurrentLevelUpInfo = LevelUpInfoDataAsset->LevelUpInformation[PlayerLevel];
		const float Range = NextLevelUpInfo.LevelUpRequirement - CurrentLevelUpInfo.LevelUpRequirement; // 현재 레벨에서 다음 레벨로 진행하기 위해 필요한 경험치량
		float Progress  = AttributeSet->GetXP() - CurrentLevelUpInfo.LevelUpRequirement; // 현재 레벨 기준으로 획득한 경험치량

		Progress = FMath::Clamp(Progress, 0.f, Range);
	
		if (Range <= 0.f)
		{
			return 0.f;
		}
	
		return Progress / Range;
		
	}

	return 0.f;
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

void ABrandNewPlayerCharacter::MoveCharacterToValidLocation(const FVector& NewLocation)
{
	const FVector ValidLocation = GetSafeTeleportLocation(NewLocation);

	SetActorLocation(ValidLocation, false, nullptr, ETeleportType::TeleportPhysics);
	SafeLocation = ValidLocation;
}

FVector ABrandNewPlayerCharacter::GetSafeTeleportLocation(const FVector& NewLocation) const
{
	const UWorld* World = GetWorld();
	if (!World) return GetActorLocation();

	FVector TraceStart = NewLocation + FVector(0.f, 0.f, 200.f);
	FVector TraceEnd = NewLocation - FVector(0.f, 0.f, 500.f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 아래로 라인 트레이스해서 바닥 찾기
	if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		FVector ValidLocation = HitResult.Location;
		ValidLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight(); // 충돌 보정
		return ValidLocation;
	}

	// 트레이스 실패 시 기존 위치 유지
	return GetActorLocation();
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
	EquippedWeaponType = CombatWeapon->GetCombatWeaponType();
	OnEquippedWeaponChanged();
}

void ABrandNewPlayerCharacter::OnWeaponUnequipped_Implementation()
{

	EquippedWeaponType = ECombatWeaponType::Unequipped;
	OnEquippedWeaponChanged();
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


float ABrandNewPlayerCharacter::GetAttributeValueByTag(const FGameplayTag& AttributeTag) const
{
	check(AttributeInfoDataAsset)
	if (!AttributeInfoDataAsset || !AttributeSet) return 0.0f;

	const FGameplayAttribute AttributeGetter = AttributeInfoDataAsset->FindAttributeGetter(AttributeTag);
	
	return AttributeGetter.GetNumericValue(AttributeSet);
	
}


void ABrandNewPlayerCharacter::K2_BroadCastCharacterInitialHealth_Implementation()
{
	MaxHealthChangedDelegate.Broadcast(AttributeSet->GetMaxHealth());
	HealthChangedDelegate.Broadcast(AttributeSet->GetHealth());
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
	if (!HasAuthority() || !AbilitySystemComponent || !AttributeSet || !GetPlayerState()) return;
	
	const UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;

	const FSaveSlotPrams SaveSlotPrams = MakeSaveSlotPrams();
	SafeLocation = SaveSlotPrams.CharacterLocation;

	const ABrandNewPlayerState* BrandNewPlayerState = CastChecked<ABrandNewPlayerState>(GetPlayerState());
	
	SaveSubsystem->SaveGameToSlotWithId(SlotName, SlotIndex, SaveSlotPrams, BrandNewPlayerState->PlayerUniqueId);
	
}

void ABrandNewPlayerCharacter::SavePlayerDataForTravel()
{
	// 해당 함수는 서버에서만 호출되지만 만약을 대비한 방어 코드
	if (!HasAuthority() || !AbilitySystemComponent || !AttributeSet || !GetPlayerState()) return;
	
	UBrandNewSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UBrandNewSaveSubsystem>();
	if (!SaveSubsystem) return;

	const ABrandNewPlayerState* BrandNewPlayerState = CastChecked<ABrandNewPlayerState>(GetPlayerState());
	
	SaveSubsystem->UpdateLatestPlayerDataMap(BrandNewPlayerState->PlayerUniqueId, MakeSaveSlotPrams());

	ABrandNewGameModeBase* BrandNewGameModeBase = Cast<ABrandNewGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!BrandNewGameModeBase) return;
	BrandNewGameModeBase->bIsWaitingForTravel = true;
	
}


FSaveSlotPrams ABrandNewPlayerCharacter::MakeSaveSlotPrams() const
{
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
	SaveSlotPrams.TitleText =  FText::FromString(TEXT("Unreal RPG Project")); // TODO: 퀘스트 시스템 구현시 변경해야함.

	// 맵 에셋 네임 저장
	const FString MapName = UWorld::RemovePIEPrefix(GetWorld()->GetOutermost()->GetName());
	SaveSlotPrams.MapPackageName = MapName;
	SaveSlotPrams.MapName = GetGameInstance<UBrandNewGameInstance>()->GetMapName(MapName);

	// 인벤토리 저장
	SaveSlotPrams.InventoryContents = GetPlayerState<ABrandNewPlayerState>()->GetInventory()->GetInventoryContents();
	
	SaveSlotPrams.bIsValid = true;

	return SaveSlotPrams;
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

void ABrandNewPlayerCharacter::UseConsumptionItem(const int32 SlotIndex)
{
	if (!AllAttributeInstanceEffect || !AbilitySystemComponent) return;

	HasAuthority() ? ConsumeItem(SlotIndex) : Server_ConsumeItem(SlotIndex);
	
}

void ABrandNewPlayerCharacter::Server_ConsumeItem_Implementation(const int32 SlotIndex)
{
	ConsumeItem(SlotIndex);
}

void ABrandNewPlayerCharacter::ConsumeItem(const int32 SlotIndex) const
{
	const ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	check(BrandNewPlayerState);
	UBrandNewInventory* Inventory = BrandNewPlayerState->GetInventory();
	if (Inventory->GetInventoryContents().EatablesSlots[SlotIndex].Quantity <= 0)
	{
		return;
	}

	const FItemDataRow ItemInfo = UBrandNewFunctionLibrary::GetItemData(this, Inventory->GetInventoryContents().EatablesSlots[SlotIndex].ItemID);
	
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AllAttributeInstanceEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Primary_Strength, ItemInfo.Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Primary_Intelligence, ItemInfo.Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Primary_Dexterity, ItemInfo.Dexterity);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Primary_Vitality, ItemInfo.Vitality);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Vital_CurrentHealth, ItemInfo.CurrentHealth);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Vital_CurrentMana, ItemInfo.CurrentMana);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	
	Inventory->ConsumeItemInSlot(EItemType::Eatable, SlotIndex);
}


void ABrandNewPlayerCharacter::UseEquipmentItem(const int32 SlotIndex, const EItemType ItemType)
{
	check(EquipmentInfiniteEffect)
	if (!AbilitySystemComponent || !EquipmentInfiniteEffect) return;

	HasAuthority() ? EquipItem(SlotIndex, ItemType) : Server_EquipItem(SlotIndex, ItemType);
	
}

void ABrandNewPlayerCharacter::Server_EquipItem_Implementation(const int32 SlotIndex, const EItemType ItemType)
{
	EquipItem(SlotIndex, ItemType);
}

void ABrandNewPlayerCharacter::EquipItem(const int32 SlotIndex, const EItemType ItemType)
{
	if (!HasAuthority()) return;
	
	const ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	check(BrandNewPlayerState);
	UBrandNewInventory* Inventory = BrandNewPlayerState->GetInventory();
	if (!Inventory) return;
	
	if (ItemType == EItemType::Weapon)
	{
		if (!Inventory->GetInventoryContents().WeaponSlots.IsValidIndex(SlotIndex) || Inventory->GetInventoryContents().WeaponSlots[SlotIndex].Quantity <= 0) return;
		
		if (ActiveWeaponEffect.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveWeaponEffect);
		}
		const int32 ItemID = Inventory->GetInventoryContents().WeaponSlots[SlotIndex].ItemID;
		ActiveWeaponEffect = ApplyInfiniteItemEffect(ItemID);

	}
	else if (ItemType == EItemType::Armor)
	{
		if (!Inventory->GetInventoryContents().ArmorSlots.IsValidIndex(SlotIndex) || Inventory->GetInventoryContents().ArmorSlots[SlotIndex].Quantity <= 0) return;
		
		if (ActiveArmorEffect.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveArmorEffect);
		}
		const int32 ItemID = Inventory->GetInventoryContents().ArmorSlots[SlotIndex].ItemID;
		ActiveArmorEffect = ApplyInfiniteItemEffect(ItemID);

	}

	Inventory->EquipItemInSlot(ItemType, SlotIndex);
}

 FActiveGameplayEffectHandle ABrandNewPlayerCharacter::ApplyInfiniteItemEffect(const int32 ItemID) const
{
	if (ItemID < 0) return FActiveGameplayEffectHandle();

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EquipmentInfiniteEffect, 1.f, ContextHandle);
	
	const FItemDataRow ItemInfo = UBrandNewFunctionLibrary::GetItemData(this, ItemID);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Item_Strength, ItemInfo.Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Item_Intelligence, ItemInfo.Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Item_Dexterity, ItemInfo.Dexterity);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, BrandNewGamePlayTag::Attribute_Item_Vitality, ItemInfo.Vitality);
	
	return AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
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


void ABrandNewPlayerCharacter::ReviveCharacter()
{
	bIsDead = false;
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		
	FGameplayTagContainer GameplayTags;
	GameplayTags.AddTag(BrandNewGamePlayTag::Ability_Shared_React_Death);
	AbilitySystemComponent->CancelAbilities(&GameplayTags);

	ApplyGameplayEffectToSelf(VitalAttributeEffect, 1.f);
	MoveCharacterToValidLocation(SafeLocation);
}

void ABrandNewPlayerCharacter::Server_ReviveCharacter_Implementation()
{
	ReviveCharacter();
}

void ABrandNewPlayerCharacter::RevivePlayerCharacter()
{
	if (!bIsDead) return;
	
	if (HasAuthority())
	{
		ReviveCharacter();
	}
	else
	{
		bIsDead = false; // 서버는 따로 설정하기 때문에 클라에서만 설정
		Server_ReviveCharacter();
	}
	
}

void ABrandNewPlayerCharacter::AddOverlappedNPC(AActor* OverlappedNPC)
{
	OverlappedActorArray.AddUnique(OverlappedNPC);
	
}

void ABrandNewPlayerCharacter::RemoveOverlappedNPC(AActor* EndOverlappedNPC)
{
	if (OverlappedActorArray.Contains(EndOverlappedNPC))
	{
		OverlappedActorArray.Remove(EndOverlappedNPC);
	}
	
}

void ABrandNewPlayerCharacter::SetCombatWeaponVisible(const bool bIsVisible)
{
	if (CombatWeapon)
	{
		CombatWeapon->SetWeaponVisible(bIsVisible);
	}
	
}

void ABrandNewPlayerCharacter::GrantQuestReward(const int32 XpReward, TMap<int32, int32> ItemRewardMap)
{
	if (!HasAuthority()) return;
	
	ApplyAddXPEffect(XpReward);
	
	const ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	UBrandNewInventory* Inventory = BrandNewPlayerState->GetInventory();

	for (const TPair<int32, int32>& Reward : ItemRewardMap)
	{
		FInventorySlotData ItemData;
		ItemData.ItemID = Reward.Key;
		ItemData.Quantity = Reward.Value;
		Inventory->AddItemToSlot(ItemData);
	}
	
}

void ABrandNewPlayerCharacter::StartDialogue(const FName& DialogueIdToStart) const
{
	if (DialogueIdToStart.IsNone()) return;
	
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	IBrandNewHUDInterface* BrandNewHUD = Cast<IBrandNewHUDInterface>(PlayerController->GetHUD());
	if (!BrandNewHUD) return;
	BrandNewHUD->HideMainOverlay();
	BrandNewHUD->CreateDialogueWidget(DialogueIdToStart);
}

void ABrandNewPlayerCharacter::InteractIfPossible()
{
	// 아이템 획득 로직
	if (HasAuthority())
	{
		if (OverlappedItems.Num() > 0)
		{
			for (const TWeakObjectPtr<AActor>& Item : OverlappedItems)
			{
				if (Item.IsValid())
				{
					// 아이템 한번에 획득 전 UI에서 위젯 제거용. 아이템은 현재 한번에 모두 획득함.
					SendPickupInfoToUi(Item.Get(), false);
				}
			}
			AcquireItem();
			return;
		}
	}
	else
	{
		if (OverlappedItemsForUI.Num() > 0)
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
			return;
		}
	}

	if (OverlappedActorArray.Num() <= 0) return;
	
	float Distance = 0.f;
	AActor* ClosestInteractiveActor = UGameplayStatics::FindNearestActor(GetActorLocation(), OverlappedActorArray, Distance);
	
	// 퀘스트 로직
	if (IQuestActorInterface* QuestActor = Cast<IQuestActorInterface>(ClosestInteractiveActor); QuestActor && GetQuestComponent())
	{
		for (const FQuestInstance& Quest : GetQuestComponent()->GetActivatedQuests())
		{
			if (QuestActor->IsQuestTargetActor(Quest.TargetId))
			{
				Server_UpdateQuestObjectiveProgress();
				if (const IBrandNewNPCInterface* NPCInterface = Cast<IBrandNewNPCInterface>(ClosestInteractiveActor))
				{
					NPCInterface->HideInteractionWidget();
				}
				StartDialogue(Quest.DialogueId);
				return;
			}
		}
	}
	
	// NPC 대화 로직
	if (const IBrandNewNPCInterface* NPCInterface = Cast<IBrandNewNPCInterface>(ClosestInteractiveActor))
	{
		// 가장 가까운 NPC 찾아서 상호작용.
		const FName FirstDialogueId = NPCInterface->GetFirstDialogueId();
		if (FirstDialogueId.IsNone()) return;
		NPCInterface->HideInteractionWidget();

		StartDialogue(FirstDialogueId);
	}
	
}

UBrandnewQuestComponent* ABrandNewPlayerCharacter::GetQuestComponent() const
{
	UActorComponent* Comp = GetPlayerStateChecked<ABrandNewPlayerState>()->GetQuestComponent();
	return Cast<UBrandnewQuestComponent>(Comp);
}

void ABrandNewPlayerCharacter::Server_UpdateQuestObjectiveProgress_Implementation()
{
	float Distance = 0.f;
	AActor* ClosestInteractiveActor = UGameplayStatics::FindNearestActor(GetActorLocation(), OverlappedActorArray, Distance);
	
	IQuestActorInterface* QuestActor = Cast<IQuestActorInterface>(ClosestInteractiveActor);
	UBrandnewQuestComponent* QuestComponent = GetQuestComponent();
	if (!QuestComponent) return;

	// 활성화 된 퀘스트를 순회하여 현재 접촉중인 대상의 타겟아이디와 퀘스트의 타겟 아이디가 동일하면 퀘스트 진행도 증가
	for (const FQuestInstance& Quest : QuestComponent->GetActivatedQuests())
	{
		if (Quest.TargetId == QuestActor->GetQuestActorId())
		{
			QuestComponent->AdvanceQuestProgress(Quest.QuestId);
		}
	}
		
}


void ABrandNewPlayerCharacter::Server_AcquireItem_Implementation()
{
	AcquireItem();
}

void ABrandNewPlayerCharacter::AcquireItem()
{
	if (OverlappedItems.IsEmpty()) return;
	
	const ABrandNewPlayerState* BrandNewPlayerState = GetPlayerState<ABrandNewPlayerState>();
	check(BrandNewPlayerState);
	UBrandNewInventory* Inventory = BrandNewPlayerState->GetInventory();
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

void ABrandNewPlayerCharacter::CameraScroll(const float InputValue)
{
	const float ScrollValue = InputValue * -CameraZoomSpeed;
	const float NewTargetArmLength =  CameraBoom->TargetArmLength + ScrollValue;
	
	TargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoom, MaxZoom);

	if (!bIsZooming)
	{
		bIsZooming = true;
		GetWorld()->GetTimerManager().SetTimer(ZoomInterpTimerHandle, this, &ThisClass::ZoomInterpTick, 0.016f, true);
	}
}

void ABrandNewPlayerCharacter::ZoomInterpTick()
{
	const float CurrentLength = CameraBoom->TargetArmLength;
	const float NewLength = FMath::FInterpTo(CurrentLength, TargetArmLength, GetWorld()->GetDeltaSeconds(), ZoomInterpSpeed);

	CameraBoom->TargetArmLength = NewLength;

	if (FMath::IsNearlyEqual(NewLength, TargetArmLength, 0.5f))
	{
		CameraBoom->TargetArmLength = TargetArmLength;
		GetWorld()->GetTimerManager().ClearTimer(ZoomInterpTimerHandle);
		bIsZooming = false;
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


