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

	void ToggleCollisionEnable(const bool bIsEnable);
	void SetWeaponVisible(const bool bIsVisible);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Brandnew|Mesh")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> WeaponCollisionBox;

	UFUNCTION()
	void CollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "BrandNew|Weapon Properties")
	ECombatWeaponType WeaponType;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> OverlappedActors;
	
public:
	FORCEINLINE ECombatWeaponType GetCombatWeaponType () const { return WeaponType; }

};
