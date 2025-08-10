// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "HunterInventoryItemDefinition.generated.h"

template <typename T> class TSubclassOf;

class UHunterInventoryItemInstance;
struct FFrame;

//////////////////////////////////////////////////////////////////////

// Represents a fragment of an item definition
UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew, Abstract)
class UHunterInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UHunterInventoryItemInstance* Instance) const {}
};

//////////////////////////////////////////////////////////////////////

/**
 * UHunterInventoryItemDefinition
 */
UCLASS(Blueprintable, Const, Abstract)
class UHunterInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UHunterInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UHunterInventoryItemFragment>> Fragments;

public:
	const UHunterInventoryItemFragment* FindFragmentByClass(TSubclassOf<UHunterInventoryItemFragment> FragmentClass) const;
};

//@TODO: Make into a subsystem instead?
UCLASS()
class UHunterInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UHunterInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, TSubclassOf<UHunterInventoryItemFragment> FragmentClass);
};
