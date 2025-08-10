// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "HunterSettingsLocal.h"
#include "HunterSettingsShared.h"
#include "Player/HunterLocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogHunterGameSettingRegistry);

#define LOCTEXT_NAMESPACE "Hunter"

//--------------------------------------
// UHunterGameSettingRegistry
//--------------------------------------

UHunterGameSettingRegistry::UHunterGameSettingRegistry()
{
}

UHunterGameSettingRegistry* UHunterGameSettingRegistry::Get(UHunterLocalPlayer* InLocalPlayer)
{
	UHunterGameSettingRegistry* Registry = FindObject<UHunterGameSettingRegistry>(InLocalPlayer, TEXT("HunterGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UHunterGameSettingRegistry>(InLocalPlayer, TEXT("HunterGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

bool UHunterGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (UHunterLocalPlayer* LocalPlayer = Cast<UHunterLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

void UHunterGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UHunterLocalPlayer* HunterLocalPlayer = Cast<UHunterLocalPlayer>(InLocalPlayer);

	VideoSettings = InitializeVideoSettings(HunterLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, HunterLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(HunterLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(HunterLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(HunterLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(HunterLocalPlayer);
	RegisterSetting(GamepadSettings);
}

void UHunterGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (UHunterLocalPlayer* LocalPlayer = Cast<UHunterLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

#undef LOCTEXT_NAMESPACE

