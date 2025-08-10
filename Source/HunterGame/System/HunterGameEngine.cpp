// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterGameEngine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterGameEngine)

class IEngineLoop;


UHunterGameEngine::UHunterGameEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHunterGameEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}

