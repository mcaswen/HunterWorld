// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterSettingValueDiscrete_MobileFPSType.h"

#include "Performance/HunterPerformanceSettings.h"
#include "Settings/HunterSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "HunterSettings"

UHunterSettingValueDiscrete_MobileFPSType::UHunterSettingValueDiscrete_MobileFPSType()
{
}

void UHunterSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const UHunterPlatformSpecificRenderingSettings* PlatformSettings = UHunterPlatformSpecificRenderingSettings::Get();
	const UHunterSettingsLocal* UserSettings = UHunterSettingsLocal::Get();

	for (int32 TestLimit : PlatformSettings->MobileFrameRateLimits)
	{
		if (UHunterSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 UHunterSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return UHunterSettingsLocal::GetDefaultMobileFrameRate();
}

FText UHunterSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

void UHunterSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void UHunterSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void UHunterSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

void UHunterSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 UHunterSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> UHunterSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

int32 UHunterSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return UHunterSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void UHunterSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	UHunterSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

#undef LOCTEXT_NAMESPACE

