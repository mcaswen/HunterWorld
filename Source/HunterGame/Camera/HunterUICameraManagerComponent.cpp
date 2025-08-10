// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterUICameraManagerComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "HunterPlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UHunterUICameraManagerComponent* UHunterUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (AHunterPlayerCameraManager* PCCamera = Cast<AHunterPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UHunterUICameraManagerComponent::UHunterUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UHunterUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UHunterUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<AHunterPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UHunterUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UHunterUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UHunterUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
