// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Actor/QuestActorInterface.h"
#include "BrandnewQuestActor.generated.h"

class UBrandnewQuestComponent;
class UWidgetComponent;

UCLASS()
class CHARACTERMODULE_API ABrandnewQuestActor : public AActor, public IQuestActorInterface
{
	GENERATED_BODY()
	
public:	
	ABrandnewQuestActor();
	
	/* begin IQuestActorInterface */
	virtual void ShowLocationWidget(const bool bIsVisible) override;
	virtual bool IsQuestTargetActor(const FName& QuestTargetId) override;
	virtual FName GetQuestActorId() const override;
	/* end IQuestActorInterface */

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Brandnew")
	FName ActorId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> LocationWidgetComponent;
	

};
