// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HunterAbilityCost.h"
#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"

#include "HunterAbilityCost_InventoryItem.generated.h"

struct FGameplayAbilityActivationInfo;
struct FGameplayAbilitySpecHandle;

class UHunterGameplayAbility;
class UHunterInventoryItemDefinition;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * Represents a cost that requires expending a quantity of an inventory item
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class UHunterAbilityCost_InventoryItem : public UHunterAbilityCost
{
	GENERATED_BODY()

public:
	UHunterAbilityCost_InventoryItem();

	//~UHunterAbilityCost interface
	virtual bool CheckCost(const UHunterGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UHunterGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UHunterAbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	TSubclassOf<UHunterInventoryItemDefinition> ItemDefinition;
};
