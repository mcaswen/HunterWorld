// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterSettingScreen.h"

#include "Input/CommonUIInputTypes.h"
#include "Player/HunterLocalPlayer.h"
#include "Settings/HunterGameSettingRegistry.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterSettingScreen)

class UGameSettingRegistry;

void UHunterSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UHunterSettingScreen::CreateRegistry()
{
	UHunterGameSettingRegistry* NewRegistry = NewObject<UHunterGameSettingRegistry>();

	if (UHunterLocalPlayer* LocalPlayer = CastChecked<UHunterLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void UHunterSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void UHunterSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void UHunterSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void UHunterSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
