// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterWeaponDebugSettings.h"
#include "Misc/App.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterWeaponDebugSettings)

UHunterWeaponDebugSettings::UHunterWeaponDebugSettings()
{
}

FName UHunterWeaponDebugSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

