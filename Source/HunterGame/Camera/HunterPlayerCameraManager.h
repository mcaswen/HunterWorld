// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/PlayerCameraManager.h"

#include "HunterPlayerCameraManager.generated.h"

class FDebugDisplayInfo;
class UCanvas;
class UObject;


#define Hunter_CAMERA_DEFAULT_FOV			(80.0f)
#define Hunter_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define Hunter_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UHunterUICameraManagerComponent;

/**
 * AHunterPlayerCameraManager
 *
 *	The base player camera manager class used by this project.
 */
UCLASS(notplaceable, MinimalAPI)
class AHunterPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AHunterPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	UHunterUICameraManagerComponent* GetUICameraComponent() const;

protected:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	TObjectPtr<UHunterUICameraManagerComponent> UICamera;
};
