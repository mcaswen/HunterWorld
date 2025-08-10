// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterPerfStatContainerBase.h"

#include "Blueprint/WidgetTree.h"
#include "HunterPerfStatWidgetBase.h"
#include "Settings/HunterSettingsLocal.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterPerfStatContainerBase)

//////////////////////////////////////////////////////////////////////
// UHunterPerfStatsContainerBase

UHunterPerfStatContainerBase::UHunterPerfStatContainerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHunterPerfStatContainerBase::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateVisibilityOfChildren();

	UHunterSettingsLocal::Get()->OnPerfStatDisplayStateChanged().AddUObject(this, &ThisClass::UpdateVisibilityOfChildren);
}

void UHunterPerfStatContainerBase::NativeDestruct()
{
	UHunterSettingsLocal::Get()->OnPerfStatDisplayStateChanged().RemoveAll(this);

	Super::NativeDestruct();
}

void UHunterPerfStatContainerBase::UpdateVisibilityOfChildren()
{
	UHunterSettingsLocal* UserSettings = UHunterSettingsLocal::Get();

	const bool bShowTextWidgets = (StatDisplayModeFilter == EHunterStatDisplayMode::TextOnly) || (StatDisplayModeFilter == EHunterStatDisplayMode::TextAndGraph);
	const bool bShowGraphWidgets = (StatDisplayModeFilter == EHunterStatDisplayMode::GraphOnly) || (StatDisplayModeFilter == EHunterStatDisplayMode::TextAndGraph);
	
	check(WidgetTree);
	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UHunterPerfStatWidgetBase* TypedWidget = Cast<UHunterPerfStatWidgetBase>(Widget))
		{
			const EHunterStatDisplayMode SettingMode = UserSettings->GetPerfStatDisplayState(TypedWidget->GetStatToDisplay());

			bool bShowWidget = false;
			switch (SettingMode)
			{
			case EHunterStatDisplayMode::Hidden:
				bShowWidget = false;
				break;
			case EHunterStatDisplayMode::TextOnly:
				bShowWidget = bShowTextWidgets;
				break;
			case EHunterStatDisplayMode::GraphOnly:
				bShowWidget = bShowGraphWidgets;
				break;
			case EHunterStatDisplayMode::TextAndGraph:
				bShowWidget = bShowTextWidgets || bShowGraphWidgets;
				break;
			}

			TypedWidget->SetVisibility(bShowWidget ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	});
}

