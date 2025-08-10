// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterPerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "Performance/HunterPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterPerformanceSettings)

//////////////////////////////////////////////////////////////////////

UHunterPlatformSpecificRenderingSettings::UHunterPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UHunterPlatformSpecificRenderingSettings* UHunterPlatformSpecificRenderingSettings::Get()
{
	UHunterPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UHunterPerformanceSettings::UHunterPerformanceSettings()
{
	PerPlatformSettings.Initialize(UHunterPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FHunterPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EHunterDisplayablePerformanceStat PerfStat : TEnumRange<EHunterDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}

