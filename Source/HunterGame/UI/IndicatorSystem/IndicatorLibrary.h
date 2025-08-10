// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "IndicatorLibrary.generated.h"

#define UE_API HUNTERGAME_API

class AController;
class UHunterIndicatorManagerComponent;
class UObject;
struct FFrame;

UCLASS(MinimalAPI)
class UIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UE_API UIndicatorLibrary();
	
	/**  */
	UFUNCTION(BlueprintCallable, Category = Indicator)
	static UE_API UHunterIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller);
};

#undef UE_API
