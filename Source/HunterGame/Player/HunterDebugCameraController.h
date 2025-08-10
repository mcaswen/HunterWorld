// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DebugCameraController.h"

#include "HunterDebugCameraController.generated.h"

class UObject;


/**
 * AHunterDebugCameraController
 *
 *	Used for controlling the debug camera when it is enabled via the cheat manager.
 */
UCLASS()
class AHunterDebugCameraController : public ADebugCameraController
{
	GENERATED_BODY()

public:

	AHunterDebugCameraController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void AddCheats(bool bForce) override;
};
