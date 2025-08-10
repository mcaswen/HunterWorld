// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Player/HunterLocalPlayer.h"
#include "Settings/HunterSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterInputComponent)

class UHunterInputConfig;

UHunterInputComponent::UHunterInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UHunterInputComponent::AddInputMappings(const UHunterInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UHunterInputComponent::RemoveInputMappings(const UHunterInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UHunterInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
