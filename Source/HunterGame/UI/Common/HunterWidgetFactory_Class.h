// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HunterWidgetFactory.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"

#include "HunterWidgetFactory_Class.generated.h"

#define UE_API HUNTERGAME_API

class UObject;
class UUserWidget;

UCLASS(MinimalAPI)
class UHunterWidgetFactory_Class : public UHunterWidgetFactory
{
	GENERATED_BODY()

public:
	UHunterWidgetFactory_Class() { }

	UE_API virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};

#undef UE_API
