// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterAbilitySystemGlobals.h"

#include "HunterGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterAbilitySystemGlobals)

struct FGameplayEffectContext;

UHunterAbilitySystemGlobals::UHunterAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UHunterAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FHunterGameplayEffectContext();
}

