// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "CharacterFunctionLibrary.h"
#include "AbilitySystem/BrandNewAttributeSet.h"
#include "BrandNewTypes/BrandNewGamePlayTag.h"


struct FBrandNewAttributeStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalMagnitude);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicDefensePower);
	
	FBrandNewAttributeStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBrandNewAttributeSet, PhysicalAttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBrandNewAttributeSet, MagicAttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBrandNewAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBrandNewAttributeSet, CriticalMagnitude, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBrandNewAttributeSet, PhysicalDefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBrandNewAttributeSet, MagicDefensePower, Target, false);
	}
	
};

static const FBrandNewAttributeStatics& GetAttributeStatics()
{
	static FBrandNewAttributeStatics BrandNewAttributeStatics;
	return BrandNewAttributeStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetAttributeStatics().PhysicalAttackPowerDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().MagicAttackPowerDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().CriticalMagnitudeDef);
	
	RelevantAttributesToCapture.Add(GetAttributeStatics().PhysicalDefensePowerDef);
	RelevantAttributesToCapture.Add(GetAttributeStatics().MagicDefensePowerDef);

	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = EffectSpec.GetEffectContext();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	/* 대미지 타입 */
	const FGameplayTag DamageTypeTag = UCharacterFunctionLibrary::GetDamageTypeTagToContext(ContextHandle);
	
	/* Source 공격력 */
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().PhysicalAttackPowerDef, EvaluateParameters, SourceAttackPower);
	
	/* 현재 사용중인 기술의 대미지 배율 */
	float DamageMagnitude = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag);
	if (DamageMagnitude > 0.f)
	{
		DamageMagnitude /= 100.f;
	}
	float BaseDamage = SourceAttackPower * DamageMagnitude;

	/* Source 치명타 확률 */
	float CriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().CriticalChanceDef, EvaluateParameters, CriticalChance);

	/* Source 치명타 배율 */
	float CriticalMagnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().CriticalMagnitudeDef, EvaluateParameters, CriticalMagnitude);
	if (CriticalMagnitude > 0.f)
	{
		CriticalMagnitude /= 100;
	}

	const bool bCriticalHit = FMath::FRandRange(1.f, 100.f) < CriticalChance;
	// 치명타 여부를 Context에 저장
	UCharacterFunctionLibrary::GetBrandNewEffectContext(ContextHandle).SetIsCriticalHit(bCriticalHit); 
	
	BaseDamage = bCriticalHit ? BaseDamage * CriticalMagnitude : BaseDamage;

	/* Target 방어력 */
	float TargetDefensePower = 0.f;
	
	if (DamageTypeTag.MatchesTagExact(BrandNewGamePlayTag::Damage_Type_Physical))
	{
		// 물리 방어력 설정
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().PhysicalDefensePowerDef, EvaluateParameters, TargetDefensePower);
	}
	else
	{
		// 마법 방어력 설정
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAttributeStatics().MagicDefensePowerDef, EvaluateParameters, TargetDefensePower);
	}

	if (TargetDefensePower > 0.f)
	{
		TargetDefensePower /= 100;
	}

	const float FinalDamage = BaseDamage * (1 - TargetDefensePower);

	const FGameplayModifierEvaluatedData EvaluatedData(UBrandNewAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, FinalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
