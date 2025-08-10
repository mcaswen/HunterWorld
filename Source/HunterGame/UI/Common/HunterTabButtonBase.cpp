// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterTabButtonBase.h"

#include "CommonLazyImage.h"
#include "UI/Common/HunterTabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterTabButtonBase)

class UObject;
struct FSlateBrush;

void UHunterTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UHunterTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UHunterTabButtonBase::SetTabLabelInfo_Implementation(const FHunterTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}

