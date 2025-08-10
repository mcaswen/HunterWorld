// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "HunterVerbMessageHelpers.generated.h"

#define UE_API HUNTERGAME_API

struct FGameplayCueParameters;
struct FHunterVerbMessage;

class APlayerController;
class APlayerState;
class UObject;
struct FFrame;


UCLASS(MinimalAPI)
class UHunterVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Hunter")
	static UE_API APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Hunter")
	static UE_API APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Hunter")
	static UE_API FGameplayCueParameters VerbMessageToCueParameters(const FHunterVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "Hunter")
	static UE_API FHunterVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};

#undef UE_API
