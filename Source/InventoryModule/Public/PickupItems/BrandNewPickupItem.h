// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Actor/PickupItemInterface.h"
#include "BrandNewPickupItem.generated.h"

class UNiagaraComponent;
class USphereComponent;

UCLASS()
class INVENTORYMODULE_API ABrandNewPickupItem : public AActor, public IPickupItemInterface
{
	GENERATED_BODY()

public:
	ABrandNewPickupItem();

	/* begin IPickupItemInterface */
	virtual int32 GetId() const override;
	virtual int32 GetQuantity() const override;
	virtual FGuid GetUniqueId() const override;
	/* end IPickupItemInterface */
	

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid ItemUniqueId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brandnew|Item Info")
	int32 ItemId = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brandnew|Item Info")
	int32 Quantity = 1;

	
};
