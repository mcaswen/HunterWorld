// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterWidgetFactory.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterWidgetFactory)

class UUserWidget;

TSubclassOf<UUserWidget> UHunterWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}
