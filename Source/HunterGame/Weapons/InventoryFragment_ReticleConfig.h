// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/HunterInventoryItemDefinition.h"

#include "InventoryFragment_ReticleConfig.generated.h"

class UHunterReticleWidgetBase;
class UObject;

UCLASS()
class UInventoryFragment_ReticleConfig : public UHunterInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Reticle)
	TArray<TSubclassOf<UHunterReticleWidgetBase>> ReticleWidgets;
};
