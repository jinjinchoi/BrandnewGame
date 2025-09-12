// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BrandNewAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CharacterFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"
#include "GameFramework/Character.h"
#include "Interfaces/BrandNewEnemyInterface.h"
#include "Interfaces/BrandNewPlayerInterface.h"
#include "Perception/AISense_Damage.h"

UBrandNewAttributeSet::UBrandNewAttributeSet()
{
	
}

void UBrandNewAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PhysicalAttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MagicAttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalMagnitude, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PhysicalDefensePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MagicDefensePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AttributePoint, COND_None, REPNOTIFY_Always);
	
}

void UBrandNewAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	
}

void UBrandNewAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Data);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP();
	}
	
}

void UBrandNewAttributeSet::HandleIncomingXP()
{
	const float LocalIncomingXP = GetIncomingXP();
	if (LocalIncomingXP <= 0.f) return;
	SetIncomingXP(0.f);
	
	IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(GetOwningActor());
	if (!PlayerInterface) return;
	SetXP(GetXP() + LocalIncomingXP); // 캐릭터 경험치 설정

	// 현재 레벨과 업할 레벨을 가져와 레벨 업 횟수를 계산
	const int32 CurrentLevel = GetCharacterLevel();
	const int32 LevelToReach = PlayerInterface->FindLevelForXP(GetXP());
	const int32 NumLevelUps = LevelToReach - CurrentLevel;

	// 레벌업 횟수 만큼 보상 찾아오는 작업 실행
	if (NumLevelUps > 0)
	{
		int32 NewLevel = CurrentLevel;
		int32 AttributePointsReward = 0;
		for (int32 i = 0; i < NumLevelUps; ++i)
		{
			++NewLevel;	
			AttributePointsReward += PlayerInterface->GetAttributePointsReward(NewLevel);
			
		}
		PlayerInterface->ApplyLevelUpGameplayEffect(LevelToReach, AttributePointsReward);
	}
}

void UBrandNewAttributeSet::HandleIncomingDamage(const struct FGameplayEffectModCallbackData& Data)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage <= 0.f) return;
	if (GetHealth() <= 0.f) return;

	const float NewHealth = FMath::Clamp(GetHealth() - LocalIncomingDamage, 0.f, GetMaxHealth());
	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

	if (NewHealth <= 0.f)
	{
		TryActivateDeathReactAbility(Data);
		SendXP(Data);
	}
	else
	{
		HandleHit(Data, LocalIncomingDamage);
		HitReportToEnemy(Data, LocalIncomingDamage);
	}
	
	ShowDamageText(Data, LocalIncomingDamage);
	
}


void UBrandNewAttributeSet::TryActivateDeathReactAbility(const struct FGameplayEffectModCallbackData& Data) const
{
	FGameplayTagContainer DeathTagContainer;
	DeathTagContainer.AddTag(BrandNewGamePlayTag::Ability_Shared_React_Death);
	Data.Target.TryActivateAbilitiesByTag(DeathTagContainer);
	
}

void UBrandNewAttributeSet::SendXP(const struct FGameplayEffectModCallbackData& Data) const
{
	const IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(Data.EffectSpec.GetContext().GetOriginalInstigator());
	const IBrandNewEnemyInterface* EnemyInterface = Cast<IBrandNewEnemyInterface>(GetOwningActor());

	if (PlayerInterface && EnemyInterface)
	{
		PlayerInterface->ApplyAddXPEffect(EnemyInterface->GetXPReward());
	}
}

void UBrandNewAttributeSet::HandleHit(const struct FGameplayEffectModCallbackData& Data, const float LocalIncomingDamage) const
{
	FGameplayEventData EventData;
	EventData.EventTag = UCharacterFunctionLibrary::GetHitDirectionTag(Data.EffectSpec.GetContext()); // 방향성 타격 구현해야함
	EventData.Instigator = Data.EffectSpec.GetContext().GetOriginalInstigator();
	EventData.Target = GetOwningActor();
	EventData.EventMagnitude = LocalIncomingDamage;

	// Hit Ability Activate
	Data.Target.HandleGameplayEvent(EventData.EventTag, &EventData);

	// 넉백
	const FVector KnockbackVector = UCharacterFunctionLibrary::GetBrandNewEffectContext(Data.EffectSpec.GetContext()).GetKnockbackImpulse();
	if (!KnockbackVector.IsNearlyZero())
	{
		if (ACharacter* TargetCharacter = Cast<ACharacter>(GetOwningActor()))
		{
			TargetCharacter->LaunchCharacter(KnockbackVector, true, true);
		}
	}
}

void UBrandNewAttributeSet::HitReportToEnemy(const struct FGameplayEffectModCallbackData& Data, const float LocalIncomingDamage) const
{
	if (!GetOwningActor()->Implements<UBrandNewEnemyInterface>() || !IsValid(Data.EffectSpec.GetContext().GetOriginalInstigator())) return;

	UAISense_Damage::ReportDamageEvent(
		GetOwningActor(),
		GetOwningActor(),
		Data.EffectSpec.GetContext().GetOriginalInstigator(),
		LocalIncomingDamage,
		Data.EffectSpec.GetContext().GetOriginalInstigator()->GetActorLocation(),
		GetOwningActor()->GetActorLocation()
	);
	
	
}

void UBrandNewAttributeSet::ShowDamageText(const struct FGameplayEffectModCallbackData& Data, const float DamageAmount) const
{
	FGameplayCueParameters GameplayCueParams;
	GameplayCueParams.RawMagnitude = DamageAmount;
	GameplayCueParams.Instigator = Data.EffectSpec.GetContext().GetInstigator();
	GameplayCueParams.Location = GetOwningActor()->GetActorLocation();
	GameplayCueParams.OriginalTag = UCharacterFunctionLibrary::GetDamageElementTagToContext(Data.EffectSpec.GetContext());

	const bool bIsCritical = UCharacterFunctionLibrary::GetBrandNewEffectContext(Data.EffectSpec.GetContext()).IsCriticalHit();
	const FGameplayTag CueTag =
		bIsCritical ? BrandNewGamePlayTag::GameplayCue_Widget_Damage_Critical : BrandNewGamePlayTag::GameplayCue_Widget_Damage_Normal;
	
	Data.Target.ExecuteGameplayCue(CueTag, GameplayCueParams);
	
}


#pragma region OnRep_Functions

void UBrandNewAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
}

void UBrandNewAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

void UBrandNewAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

void UBrandNewAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}

void UBrandNewAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UBrandNewAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UBrandNewAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Dexterity, OldDexterity);
}

void UBrandNewAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vitality, OldVitality);
}

void UBrandNewAttributeSet::OnRep_PhysicalAttackPower(const FGameplayAttributeData& OldPhysicalAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PhysicalAttackPower, OldPhysicalAttackPower);
}

void UBrandNewAttributeSet::OnRep_MagicAttackPower(const FGameplayAttributeData& OldMagicAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MagicAttackPower, OldMagicAttackPower);
}

void UBrandNewAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalChance, OldCriticalChance);
}

void UBrandNewAttributeSet::OnRep_CriticalMagnitude(const FGameplayAttributeData& OldCriticalMagnitude)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalMagnitude, OldCriticalMagnitude);
}

void UBrandNewAttributeSet::OnRep_PhysicalDefensePower(const FGameplayAttributeData& OldPhysicalDefensePower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, PhysicalDefensePower, OldPhysicalDefensePower);
}

void UBrandNewAttributeSet::OnRep_MagicDefensePower(const FGameplayAttributeData& OldMagicDefensePower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MagicDefensePower, OldMagicDefensePower);
}

void UBrandNewAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CharacterLevel, OldLevel);
}

void UBrandNewAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, XP, OldXP);
	
}

void UBrandNewAttributeSet::OnRep_AttributePoint(const FGameplayAttributeData& OldAttributePoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AttributePoint, OldAttributePoint);
}
#pragma endregion 
