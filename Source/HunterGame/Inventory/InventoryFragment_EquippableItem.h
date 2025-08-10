// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/HunterInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "InventoryFragment_EquippableItem.generated.h"

class UHunterEquipmentDefinition;
class UObject;

UCLASS()
class UInventoryFragment_EquippableItem : public UHunterInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Hunter)
	TSubclassOf<UHunterEquipmentDefinition> EquipmentDefinition;
};
