// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterRuntimeOptions.h"

#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterRuntimeOptions)

UHunterRuntimeOptions::UHunterRuntimeOptions()
{
	OptionCommandPrefix = TEXT("ro");
}

UHunterRuntimeOptions* UHunterRuntimeOptions::GetRuntimeOptions()
{
	return GetMutableDefault<UHunterRuntimeOptions>();
}

const UHunterRuntimeOptions& UHunterRuntimeOptions::Get()
{
	const UHunterRuntimeOptions& RuntimeOptions = *GetDefault<UHunterRuntimeOptions>();
	return RuntimeOptions;
}
