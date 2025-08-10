// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/Foundation/HunterLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// UHunterLoadingScreenSubsystem

UHunterLoadingScreenSubsystem::UHunterLoadingScreenSubsystem()
{
}

void UHunterLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> UHunterLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}

