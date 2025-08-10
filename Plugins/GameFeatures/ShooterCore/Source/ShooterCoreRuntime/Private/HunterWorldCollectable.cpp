// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterWorldCollectable.h"

#include "Async/TaskGraphInterfaces.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterWorldCollectable)

struct FInteractionQuery;

AHunterWorldCollectable::AHunterWorldCollectable()
{
}

void AHunterWorldCollectable::GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInventoryPickup AHunterWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}
