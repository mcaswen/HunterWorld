// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterAimSensitivityData.h"

#include "Settings/HunterSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterAimSensitivityData)

UHunterAimSensitivityData::UHunterAimSensitivityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SensitivityMap =
	{
		{ EHunterGamepadSensitivity::Slow,			0.5f },
		{ EHunterGamepadSensitivity::SlowPlus,		0.75f },
		{ EHunterGamepadSensitivity::SlowPlusPlus,	0.9f },
		{ EHunterGamepadSensitivity::Normal,		1.0f },
		{ EHunterGamepadSensitivity::NormalPlus,	1.1f },
		{ EHunterGamepadSensitivity::NormalPlusPlus,1.25f },
		{ EHunterGamepadSensitivity::Fast,			1.5f },
		{ EHunterGamepadSensitivity::FastPlus,		1.75f },
		{ EHunterGamepadSensitivity::FastPlusPlus,	2.0f },
		{ EHunterGamepadSensitivity::Insane,		2.5f },
	};
}

const float UHunterAimSensitivityData::SensitivtyEnumToFloat(const EHunterGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}

