// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterCameraMode_TopDownArenaCamera.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterCameraMode_TopDownArenaCamera)

UHunterCameraMode_TopDownArenaCamera::UHunterCameraMode_TopDownArenaCamera()
{
	ArenaWidth = 1000.0f;
	ArenaHeight = 1000.0f;
}

void UHunterCameraMode_TopDownArenaCamera::UpdateView(float DeltaTime)
{
	FBox ArenaBounds(FVector(-ArenaWidth, -ArenaHeight, 0.0f), FVector(ArenaWidth, ArenaHeight, 100.0f));

	const double BoundsMaxComponent = ArenaBounds.GetSize().GetMax();

	const double CameraLoftDistance = BoundsSizeToDistance.GetRichCurveConst()->Eval(BoundsMaxComponent);
	
	FVector PivotLocation = ArenaBounds.GetCenter() - DefaultPivotRotation.Vector() * CameraLoftDistance;
	
	FRotator PivotRotation = DefaultPivotRotation;

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

