// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetTypeActions_HunterContextEffectsLibrary.h"

#include "Feedback/ContextEffects/HunterContextEffectsLibrary.h"

class UClass;

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_HunterContextEffectsLibrary::GetSupportedClass() const
{
	return UHunterContextEffectsLibrary::StaticClass();
}

#undef LOCTEXT_NAMESPACE
