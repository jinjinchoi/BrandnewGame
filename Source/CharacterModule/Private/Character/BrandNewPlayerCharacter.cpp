// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BrandNewPlayerCharacter.h"

#include "DebugHelper.h"
#include "AbilitySystem/BrandNewAbilitySystemComponent.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/DataAsset_DefaultPlayerAbilities.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/BnPlayerControllerInterface.h"
#include "Interfaces/BrandNewHUDInterface.h"
#include "Interfaces/BrandNewPlayerAnimInterface.h"
#include "Item/Equipment/BrandNewWeapon.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/HUD.h"


ABrandNewPlayerCharacter::ABrandNewPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
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


#if WITH_EDITOR // === 디버그 라인 ===
		
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0.f, 0, 2.f);
		DrawDebugSphere(GetWorld(), End, Radius, 16, LineColor, false, 0.f);

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 16, FColor::Yellow, false, 2.f);
		}
		
#endif
		
	}
	
}


void ABrandNewPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnEquippedWeaponChanged();
	
}


void ABrandNewPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Server_SetMovementMode(CurrentGate);
	InitAbilityActorInfo();
	BindAttributeDelegates();
	InitHUDAndBroadCastInitialValue();
	AddCharacterAbilities();
}

void ABrandNewPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilityActorInfo(); // ASC를 초기화 하고 기본 능력치 적용
	BindAttributeDelegates(); // 그후 Attribute 변화를 바인딩(위젯에 알리기 위한 용도)
	InitHUDAndBroadCastInitialValue(); // 바인딩이 끝났으면 HUD 초기화 요청, HUD에서는 위젯 구성하고 위젯에서 초기값을 요청함.
}

void ABrandNewPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGate);
	DOREPLIFETIME(ThisClass, EquippedWeaponType);
	
}

void ABrandNewPlayerCharacter::AddCharacterAbilities() const
{
	if (!HasAuthority() || DefaultAbilitiesDataAsset.IsNull() || !AbilitySystemComponent) return;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(
		DefaultAbilitiesDataAsset.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this]()
		{
			const UDataAsset_DefaultPlayerAbilities* LoadedData = DefaultAbilitiesDataAsset.Get();
			if (IsValid(LoadedData) && IsValid(AbilitySystemComponent))
			{
				AbilitySystemComponent->GrantAbilities(LoadedData->PassiveAbilities, true);
				AbilitySystemComponent->GrantPlayerInputAbilities(LoadedData->InputAbilities);
			}
			
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

void ABrandNewPlayerCharacter::OnRep_CurrentEquippedWeaponType()
{
	OnEquippedWeaponChanged();
}

void ABrandNewPlayerCharacter::OnEquippedWeaponChanged()
{
	// 무기 타입별 애님 레이어를 변경 
	if (WeaponAnimLayerMap.Contains(EquippedWeaponType))
	{
		GetMesh()->LinkAnimClassLayers(WeaponAnimLayerMap[EquippedWeaponType]);
	}

	// 인풋 매핑 컨텍스트를 변경. 매핑 컨텍스트는 로컬 컨트롤러에서만 바꾸는 것이 의미 있기 때문에 확인해야함. (이 함수는 서버 클라이언트 모두에게 실행됨)
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
		
	}

	LastEquippedWeaponType = EquippedWeaponType;
	
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

void ABrandNewPlayerCharacter::RequestBroadCastAttributeValue()
{
	MaxHealthChangedDelegate.ExecuteIfBound(AttributeSet->GetMaxHealth());
	HealthChangedDelegate.ExecuteIfBound(AttributeSet->GetHealth());
	MaxManaChangedDelegate.ExecuteIfBound(AttributeSet->GetMaxMana());
	ManaChangedDelegate.ExecuteIfBound(AttributeSet->GetMana());
}


#pragma region Movement
void ABrandNewPlayerCharacter::Server_SetMovementMode_Implementation(const EGate NewGate)
{
	if (!HasAuthority()) return;
	
	CurrentGate = (CurrentGate == NewGate) ? EGate::Jogging : NewGate;
	
	UpdateMovementComponentPrams();

	if (IBrandNewPlayerAnimInterface* AnimInterface = Cast<IBrandNewPlayerAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->UpdateCurrentGate(CurrentGate);
	}
	
}

void ABrandNewPlayerCharacter::OnRep_CurrentGate()
{
	UpdateMovementComponentPrams();
	
	if (IBrandNewPlayerAnimInterface* AnimInterface = Cast<IBrandNewPlayerAnimInterface>(GetMesh()->GetAnimInstance()))
	{
		AnimInterface->UpdateCurrentGate(CurrentGate);
	}
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
}
#pragma endregion

void ABrandNewPlayerCharacter::AddYawRotation(const float DeltaYaw)
{
	FRotator NewRot = GetActorRotation();
	NewRot.Yaw += DeltaYaw;
	SetActorRotation(NewRot);
}