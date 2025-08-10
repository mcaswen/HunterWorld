// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

HUNTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogHunter, Log, All);
HUNTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogHunterExperience, Log, All);
HUNTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogHunterAbilitySystem, Log, All);
HUNTERGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogHunterTeams, Log, All);

HUNTERGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
