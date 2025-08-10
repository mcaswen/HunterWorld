// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterDebugCameraController.h"
#include "HunterCheatManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterDebugCameraController)


AHunterDebugCameraController::AHunterDebugCameraController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use the same cheat class as HunterPlayerController to allow toggling the debug camera through cheats.
	CheatClass = UHunterCheatManager::StaticClass();
}

void AHunterDebugCameraController::AddCheats(bool bForce)
{
	// Mirrors HunterPlayerController's AddCheats() to avoid the player becoming stuck in the debug camera.
#if USING_CHEAT_MANAGER
	Super::AddCheats(true);
#else //#if USING_CHEAT_MANAGER
	Super::AddCheats(bForce);
#endif // #else //#if USING_CHEAT_MANAGER
}

