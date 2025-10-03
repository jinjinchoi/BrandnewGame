// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/BrandNewInventory.h"

#include "FunctionLibrary/BrandNewFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


UBrandNewInventory::UBrandNewInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UBrandNewInventory::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ItemInventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, LastEquippedWeaponSlotIndex, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, LastEquippedArmorSlotIndex, COND_OwnerOnly);
	
}

void UBrandNewInventory::AddItemToSlot(const FInventorySlotData& NewItem)
{
	const FItemDataRow ItemInfo = UBrandNewFunctionLibrary::GetItemData(this, NewItem.ItemID);

	switch (ItemInfo.ItemType)
	{
		case EItemType::None:
			return;
		
		case EItemType::Weapon:
			ItemInventory.WeaponSlots.Add(NewItem);
			break;
		
		case EItemType::Armor:
			ItemInventory.ArmorSlots.Add(NewItem);
			break;
		
		case EItemType::Eatable:
			StackItemIntoInventory(NewItem);
			break;
		
		default:
			break;
	}
	
}

void UBrandNewInventory::ConsumeItemInSlot(const EItemType ItemType, const int32 SlotIndex, const int32 NumOfRemoval)
{
	TArray<FInventorySlotData>* Slots = nullptr;

	switch (ItemType)
	{
	case EItemType::None:
		return;
	case EItemType::Weapon:
		Slots = &ItemInventory.WeaponSlots;
		break;
	case EItemType::Armor:
		Slots = &ItemInventory.ArmorSlots;
		break;
	case EItemType::Eatable:
		Slots = &ItemInventory.EatablesSlots;
		break;
	default:
		return;
	}

	if (!Slots || !Slots->IsValidIndex(SlotIndex)) return;

	FInventorySlotData& SlotData = (*Slots)[SlotIndex];
	
	SlotData.Quantity = FMath::Max(0, SlotData.Quantity - NumOfRemoval);

	if (SlotData.Quantity <= 0)
	{
		Slots->RemoveAt(SlotIndex);
	}
	
	
}

void UBrandNewInventory::EquipItemInSlot(const EItemType ItemType, const int32 SlotIndex)
{
	if (ItemType == EItemType::Weapon && SlotIndex != LastEquippedWeaponSlotIndex)
	{
		if (LastEquippedWeaponSlotIndex != INDEX_NONE && ItemInventory.WeaponSlots.IsValidIndex(LastEquippedWeaponSlotIndex))
		{
			ItemInventory.WeaponSlots[LastEquippedWeaponSlotIndex].bIsEquipped = false;
		}
		ItemInventory.WeaponSlots[SlotIndex].bIsEquipped = true;
		LastEquippedWeaponSlotIndex = SlotIndex;
	}
	else if (ItemType == EItemType::Armor && SlotIndex != LastEquippedArmorSlotIndex)
	{
		if (LastEquippedArmorSlotIndex != INDEX_NONE && ItemInventory.ArmorSlots.IsValidIndex(LastEquippedArmorSlotIndex))
		{
			ItemInventory.ArmorSlots[LastEquippedArmorSlotIndex].bIsEquipped = false;
		}
		ItemInventory.ArmorSlots[SlotIndex].bIsEquipped = true;
		LastEquippedArmorSlotIndex = SlotIndex;
	}
	
}

void UBrandNewInventory::StackItemIntoInventory(const FInventorySlotData& NewItem)
{
	const FItemDataRow ItemInfo = UBrandNewFunctionLibrary::GetItemData(this, NewItem.ItemID);
	const int32 MaxStack = ItemInfo.StackSize;
	int32 RemainingItemCount = NewItem.Quantity;

	// 슬롯에 동일한 아이템이 있는지 확인하고 존재하면 남은 Stack Size에 맞게 아이템 쌓기
	for (FInventorySlotData& EatableSlot :  ItemInventory.EatablesSlots)
	{
		// 동일 아이템이 아니면 다음 슬롯으로 이동.
		if (EatableSlot.ItemID != NewItem.ItemID) continue;

		// 남은 공간이 없으면 다음 슬롯으로 이동.
		const int32 AvailableSpace = MaxStack - EatableSlot.Quantity;
		if (AvailableSpace <= 0) continue;

		// 남은 공간과 남은 아이템 중 더 적은 양을 슬롯에 추가.
		const int32 ToAdd = FMath::Min(RemainingItemCount, AvailableSpace);
		EatableSlot.Quantity += ToAdd;
		RemainingItemCount -= ToAdd;

		// 슬롯에 추가하고 남은 아이템이 없으면 함수 종료
		if (RemainingItemCount <= 0)
		{
			return;
		}
	}
	// 남은 수량이 있다면 새 슬롯에 나눠서 추가
	while (RemainingItemCount > 0)
	{
		// 추가할 아이템 수 계산.
		// 남은 아이템 수보다 슬롯의 스택 사이즈가 적으면 스택 사이즈만큼 추가. 그 외에는 남은 아이템 전부 추가
		const int32 ToAdd = FMath::Min(RemainingItemCount, MaxStack); 
		
		FInventorySlotData NewSlot;
		NewSlot.ItemID = NewItem.ItemID;
		NewSlot.Quantity = ToAdd;

		// 새로운 슬롯에 아이템 추가
		ItemInventory.EatablesSlots.Add(NewSlot);
		// 남은 아이템 수 갱신
		RemainingItemCount -= ToAdd;
	}
	
	
}

void UBrandNewInventory::SetInventoryContents(const FInventoryContents& NewContents)
{
	ItemInventory = NewContents;

	int32 Index = 0;

	for (const FInventorySlotData& Slot : ItemInventory.WeaponSlots)
	{
		if (Slot.bIsEquipped)
		{
			LastEquippedWeaponSlotIndex = Index;
			break;
		}
		++Index;
	}

	Index = 0;

	for (const FInventorySlotData& Slot : ItemInventory.ArmorSlots)
	{
		if (Slot.bIsEquipped)
		{
			LastEquippedArmorSlotIndex = Index;
			break;
		}
		++Index;
	}
	
}

