// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterCombatSet.h"

#include "AbilitySystem/Attributes/HunterAttributeSet.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterCombatSet)

class FLifetimeProperty;


UHunterCombatSet::UHunterCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UHunterCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHunterCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHunterCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UHunterCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHunterCombatSet, BaseDamage, OldValue);
}

void UHunterCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHunterCombatSet, BaseHeal, OldValue);
}

