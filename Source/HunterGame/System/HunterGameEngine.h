// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/GameEngine.h"

#include "HunterGameEngine.generated.h"

class IEngineLoop;
class UObject;


UCLASS()
class UHunterGameEngine : public UGameEngine
{
	GENERATED_BODY()

public:

	UHunterGameEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init(IEngineLoop* InEngineLoop) override;
};
