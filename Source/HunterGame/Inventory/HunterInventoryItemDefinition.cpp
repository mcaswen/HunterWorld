// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterInventoryItemDefinition.h"

#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterInventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// UHunterInventoryItemDefinition

UHunterInventoryItemDefinition::UHunterInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UHunterInventoryItemFragment* UHunterInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UHunterInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UHunterInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UHunterInventoryItemDefinition

const UHunterInventoryItemFragment* UHunterInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, TSubclassOf<UHunterInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UHunterInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

