// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/Object.h"

#include "HunterWidgetFactory.generated.h"

#define UE_API HUNTERGAME_API

template <class TClass> class TSubclassOf;

class UUserWidget;
struct FFrame;

UCLASS(MinimalAPI, Abstract, Blueprintable, BlueprintType, EditInlineNew)
class UHunterWidgetFactory : public UObject
{
	GENERATED_BODY()

public:
	UHunterWidgetFactory() { }

	UFUNCTION(BlueprintNativeEvent)
	UE_API TSubclassOf<UUserWidget> FindWidgetClassForData(const UObject* Data) const;
};

#undef UE_API
