// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterAttributeSet.h"

#include "AbilitySystem/HunterAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterAttributeSet)

class UWorld;


UHunterAttributeSet::UHunterAttributeSet()
{
}

UWorld* UHunterAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UHunterAbilitySystemComponent* UHunterAttributeSet::GetHunterAbilitySystemComponent() const
{
	return Cast<UHunterAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

