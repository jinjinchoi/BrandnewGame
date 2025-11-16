// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTableStruct/DataTableRowStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFunctionLibrary.generated.h"

class UQuestWidgetController;
class UGameOverWidgetController;
class UInventoryWidgetController;
class UCharacterInfoWidgetController;

USTRUCT(BlueprintType)
struct FSaveSlotViewInfoParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText TitleText = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	FText Date = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	int32 Level = 1;

	UPROPERTY(BlueprintReadOnly)
	FText LocationName = FText::GetEmpty();

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid = false;	
	
};

/**
 * 
 */
UCLASS()
class UIMODULE_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "UIFunctionLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UCharacterInfoWidgetController* GetCharacterInfoWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "UIFunctionLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "UIFunctionLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UGameOverWidgetController* GetGameOverWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "UIFunctionLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UQuestWidgetController* GetQuestWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "UIFunctionLibrary|SaveLogic", meta=(DefaultToSelf = "WorldContextObject"))
	static FSaveSlotViewInfoParams GetSaveSlotInfo(const UObject* WorldContextObject, const FString& SlotName, const int32 SlotIndex = 1);

	UFUNCTION(BlueprintCallable, Category = "UIFunctionLibrary|SaveLogic")
	static void RequestSave(ACharacter* PlayerCharacter, const FString& SlotName, const int32 SlotIndex = 1);

	UFUNCTION(BlueprintPure, Category = "UIFunctionLibrary|UI")
	static FText GetFormattedItemDescription(const FText& OriginalDescription, const FItemDataRow& ItemData);

	template<typename T>
	static FFormatNamedArguments MakeFormatArgsFromDataTableRow(const T& Struct);


};

template <typename T>
FFormatNamedArguments UUIFunctionLibrary::MakeFormatArgsFromDataTableRow(const T& Struct)
{
	FFormatNamedArguments Args;
	const UStruct* StructType = T::StaticStruct();

	// 특정 Struct나 Class의 모든 리플렉션 가능한 필드 (UPROPERTY)를 순회.
	for (TFieldIterator<FProperty> It(StructType); It; ++It)
	{
		// FProperty는 엔진에서 리플렉션을 위하여 멤버변수들의 타입/이름/메타데이터를 보관하는 객체
		FProperty* Property = *It;
		FString PropName = Property->GetName();

		// Float Type의 Property를 처리
		if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
		{
			// T타입의 변수(여기서는 구조체)에서 값을 가져옴.
			const float Value = FloatProperty->GetPropertyValue_InContainer(&Struct);
			Args.Add(*PropName, FText::AsNumber(Value));
		}
	}

	return Args;
	
}
