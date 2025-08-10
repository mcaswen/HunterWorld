// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/HunterGameplayAbility.h"

#include "HunterGameplayAbility_FromEquipment.generated.h"

class UHunterEquipmentInstance;
class UHunterInventoryItemInstance;

/**
 * UHunterGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UHunterGameplayAbility_FromEquipment : public UHunterGameplayAbility
{
	GENERATED_BODY()

public:

	UHunterGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Hunter|Ability")
	UHunterEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Hunter|Ability")
	UHunterInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
