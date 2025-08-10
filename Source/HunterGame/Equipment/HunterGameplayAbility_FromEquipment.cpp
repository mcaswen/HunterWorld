// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterGameplayAbility_FromEquipment.h"
#include "HunterEquipmentInstance.h"
#include "Inventory/HunterInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterGameplayAbility_FromEquipment)

UHunterGameplayAbility_FromEquipment::UHunterGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UHunterEquipmentInstance* UHunterGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UHunterEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UHunterInventoryItemInstance* UHunterGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UHunterEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UHunterInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UHunterGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Context.AddError(NSLOCTEXT("Hunter", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
