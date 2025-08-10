#pragma once

#include "Camera/HunterCameraMode.h"
#include "Curves/CurveFloat.h"

#include "HunterCameraMode_TopDownArenaCamera.generated.h"

class UObject;


/**
 * UHunterCameraMode_TopDownArenaCamera
 *
 *	A basic third person camera mode that looks down at a fixed arena.
 */
UCLASS(Abstract, Blueprintable)
class UHunterCameraMode_TopDownArenaCamera : public UHunterCameraMode
{
	GENERATED_BODY()

public:

	UHunterCameraMode_TopDownArenaCamera();

protected:

	//~UHunterCameraMode interface
	virtual void UpdateView(float DeltaTime) override;
	//~End of UHunterCameraMode interface

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	float ArenaWidth;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	float ArenaHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FRotator DefaultPivotRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FRuntimeFloatCurve BoundsSizeToDistance;
};
