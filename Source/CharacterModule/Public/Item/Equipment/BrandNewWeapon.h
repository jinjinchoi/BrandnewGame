// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrandNewTypes/BrandNewEnumTypes.h"
#include "GameFramework/Actor.h"
#include "BrandNewWeapon.generated.h"

class UBoxComponent;

UCLASS()
class CHARACTERMODULE_API ABrandNewWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABrandNewWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

private:
	UPROPERTY(EditAnywhere, Category = "BrandNew|Weapon Properties")
	ECombatWeaponType WeaponType;

public:
	FORCEINLINE ECombatWeaponType GetCombatWeaponType () const { return WeaponType; }

};
