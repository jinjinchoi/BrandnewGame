#pragma once

#include "GameplayEffectTypes.h"
#include "BrandNewAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FBrandNewEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

#pragma region UnrealSyntax
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FBrandNewEffectContext* Duplicate() const override
	{
		FBrandNewEffectContext* NewContext = new FBrandNewEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
#pragma endregion 


protected:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	FVector KnockbackImpulse = FVector::ZeroVector;
	
	TSharedPtr<FGameplayTag> DamageTypeTag;
	TSharedPtr<FGameplayTag> DamageElementTag;
	TSharedPtr<FGameplayTag> HitDirectionTag;

public:
	void SetIsCriticalHit(const bool InIsCriticalHit) { bIsCriticalHit = InIsCriticalHit; }
	void SetDamageTypeTag(const TSharedPtr<FGameplayTag>& InDamageType) { DamageTypeTag = InDamageType; }
	void SetDamageElementTag(const TSharedPtr<FGameplayTag>& InElementType) { DamageElementTag = InElementType; }
	void SetHitDirectionTag(const TSharedPtr<FGameplayTag>& TagToSet) { HitDirectionTag = TagToSet; }
	void SetKnockbackImpulse(const FVector& VectorToSet) { KnockbackImpulse = VectorToSet; }

	bool IsCriticalHit() const { return bIsCriticalHit; }
	TSharedPtr<FGameplayTag> GetDamageTypeTag() const { return DamageTypeTag; }
	TSharedPtr<FGameplayTag> GetDamageElementTag() const { return DamageElementTag; }
	TSharedPtr<FGameplayTag> GetHitDirectionTag() const { return HitDirectionTag; }
	FVector GetKnockbackImpulse() const { return KnockbackImpulse; }
	
};

template<>
struct TStructOpsTypeTraits<FBrandNewEffectContext> : public TStructOpsTypeTraitsBase2<FBrandNewEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};