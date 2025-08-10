// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HunterCameraComponent.h"
#include "HunterUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

AHunterPlayerCameraManager::AHunterPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = Hunter_CAMERA_DEFAULT_FOV;
	ViewPitchMin = Hunter_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = Hunter_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UHunterUICameraManagerComponent>(UICameraComponentName);
}

UHunterUICameraManagerComponent* AHunterPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void AHunterPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AHunterPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("HunterPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UHunterCameraComponent* CameraComponent = UHunterCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}

