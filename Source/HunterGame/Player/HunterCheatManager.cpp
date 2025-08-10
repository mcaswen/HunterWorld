// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterCheatManager.h"
#include "GameFramework/Pawn.h"
#include "HunterPlayerController.h"
#include "HunterDebugCameraController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Console.h"
#include "GameFramework/HUD.h"
#include "System/HunterAssetManager.h"
#include "System/LyraGameData.h"
#include "HunterGameplayTags.h"
#include "AbilitySystem/HunterAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Character/HunterHealthComponent.h"
#include "Character/HunterPawnExtensionComponent.h"
#include "System/HunterSystemStatics.h"
#include "Development/HunterDeveloperSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterCheatManager)

DEFINE_LOG_CATEGORY(LogHunterCheat);

namespace HunterCheat
{
	static const FName NAME_Fixed = FName(TEXT("Fixed"));
	
	static bool bEnableDebugCameraCycling = false;
	static FAutoConsoleVariableRef CVarEnableDebugCameraCycling(
		TEXT("HunterCheat.EnableDebugCameraCycling"),
		bEnableDebugCameraCycling,
		TEXT("If true then you can cycle the debug camera while running the game."),
		ECVF_Cheat);

	static bool bStartInGodMode = false;
	static FAutoConsoleVariableRef CVarStartInGodMode(
		TEXT("HunterCheat.StartInGodMode"),
		bStartInGodMode,
		TEXT("If true then the God cheat will be applied on begin play"),
		ECVF_Cheat);
};


UHunterCheatManager::UHunterCheatManager()
{
	DebugCameraControllerClass = AHunterDebugCameraController::StaticClass();
}

void UHunterCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

#if WITH_EDITOR
	if (GIsEditor)
	{
		APlayerController* PC = GetOuterAPlayerController();
		for (const FHunterCheatToRun& CheatRow : GetDefault<UHunterDeveloperSettings>()->CheatsToRun)
		{
			if (CheatRow.Phase == ECheatExecutionTime::OnCheatManagerCreated)
			{
				PC->ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
			}
		}
	}
#endif

	if (HunterCheat::bStartInGodMode)
	{
		God();	
	}
}

void UHunterCheatManager::CheatOutputText(const FString& TextToOutput)
{
#if USING_CHEAT_MANAGER
	// Output to the console.
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportConsole)
	{
		GEngine->GameViewport->ViewportConsole->OutputText(TextToOutput);
	}

	// Output to log.
	UE_LOG(LogHunterCheat, Display, TEXT("%s"), *TextToOutput);
#endif // USING_CHEAT_MANAGER
}

void UHunterCheatManager::Cheat(const FString& Msg)
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
		HunterPC->ServerCheat(Msg.Left(128));
	}
}

void UHunterCheatManager::CheatAll(const FString& Msg)
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
		HunterPC->ServerCheatAll(Msg.Left(128));
	}
}

void UHunterCheatManager::PlayNextGame()
{
	UHunterSystemStatics::PlayNextGame(this);
}

void UHunterCheatManager::EnableDebugCamera()
{
	Super::EnableDebugCamera();
}

void UHunterCheatManager::DisableDebugCamera()
{
	FVector DebugCameraLocation;
	FRotator DebugCameraRotation;

	ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* OriginalPC = nullptr;

	if (DebugCC)
	{
		OriginalPC = DebugCC->OriginalControllerRef;
		DebugCC->GetPlayerViewPoint(DebugCameraLocation, DebugCameraRotation);
	}

	Super::DisableDebugCamera();

	if (OriginalPC && OriginalPC->PlayerCameraManager && (OriginalPC->PlayerCameraManager->CameraStyle == HunterCheat::NAME_Fixed))
	{
		OriginalPC->SetInitialLocationAndRotation(DebugCameraLocation, DebugCameraRotation);

		OriginalPC->PlayerCameraManager->ViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->ViewTarget.POV.Rotation = DebugCameraRotation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Location = DebugCameraLocation;
		OriginalPC->PlayerCameraManager->PendingViewTarget.POV.Rotation = DebugCameraRotation;
	}
}

bool UHunterCheatManager::InDebugCamera() const
{
	return (Cast<ADebugCameraController>(GetOuter()) ? true : false);
}

void UHunterCheatManager::EnableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(HunterCheat::NAME_Fixed);
	}
}

void UHunterCheatManager::DisableFixedCamera()
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		PC->SetCameraMode(NAME_Default);
	}
}

bool UHunterCheatManager::InFixedCamera() const
{
	const ADebugCameraController* DebugCC = Cast<ADebugCameraController>(GetOuter());
	const APlayerController* PC = (DebugCC ? ToRawPtr(DebugCC->OriginalControllerRef) : GetOuterAPlayerController());

	if (PC && PC->PlayerCameraManager)
	{
		return (PC->PlayerCameraManager->CameraStyle == HunterCheat::NAME_Fixed);
	}

	return false;
}

void UHunterCheatManager::ToggleFixedCamera()
{
	if (InFixedCamera())
	{
		DisableFixedCamera();
	}
	else
	{
		EnableFixedCamera();
	}
}

void UHunterCheatManager::CycleDebugCameras()
{
	if (!HunterCheat::bEnableDebugCameraCycling)
	{
		return;
	}
	
	if (InDebugCamera())
	{
		EnableFixedCamera();
		DisableDebugCamera();
	}
	else if (InFixedCamera())
	{
		DisableFixedCamera();
		DisableDebugCamera();
	}
	else
	{
		EnableDebugCamera();
		DisableFixedCamera();
	}
}

void UHunterCheatManager::CycleAbilitySystemDebug()
{
	APlayerController* PC = Cast<APlayerController>(GetOuterAPlayerController());

	if (PC && PC->MyHUD)
	{
		if (!PC->MyHUD->bShowDebugInfo || !PC->MyHUD->DebugDisplay.Contains(TEXT("AbilitySystem")))
		{
			PC->MyHUD->ShowDebug(TEXT("AbilitySystem"));
		}

		PC->ConsoleCommand(TEXT("AbilitySystem.Debug.NextCategory"));
	}
}

void UHunterCheatManager::CancelActivatedAbilities()
{
	if (UHunterAbilitySystemComponent* HunterASC = GetPlayerAbilitySystemComponent())
	{
		const bool bReplicateCancelAbility = true;
		HunterASC->CancelInputActivatedAbilities(bReplicateCancelAbility);
	}
}

void UHunterCheatManager::AddTagToSelf(FString TagName)
{
	FGameplayTag Tag = HunterGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UHunterAbilitySystemComponent* HunterASC = GetPlayerAbilitySystemComponent())
		{
			HunterASC->AddDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogHunterCheat, Display, TEXT("AddTagToSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UHunterCheatManager::RemoveTagFromSelf(FString TagName)
{
	FGameplayTag Tag = HunterGameplayTags::FindTagByString(TagName, true);
	if (Tag.IsValid())
	{
		if (UHunterAbilitySystemComponent* HunterASC = GetPlayerAbilitySystemComponent())
		{
			HunterASC->RemoveDynamicTagGameplayEffect(Tag);
		}
	}
	else
	{
		UE_LOG(LogHunterCheat, Display, TEXT("RemoveTagFromSelf: Could not find any tag matching [%s]."), *TagName);
	}
}

void UHunterCheatManager::DamageSelf(float DamageAmount)
{
	if (UHunterAbilitySystemComponent* HunterASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerDamage(HunterASC, DamageAmount);
	}
}

void UHunterCheatManager::DamageTarget(float DamageAmount)
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
		if (HunterPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			HunterPC->ServerCheat(FString::Printf(TEXT("DamageTarget %.2f"), DamageAmount));
			return;
		}

		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(HunterPC, TargetHitResult);

		if (UHunterAbilitySystemComponent* HunterTargetASC = Cast<UHunterAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerDamage(HunterTargetASC, DamageAmount);
		}
	}
}

void UHunterCheatManager::ApplySetByCallerDamage(UHunterAbilitySystemComponent* HunterASC, float DamageAmount)
{
	check(HunterASC);

	TSubclassOf<UGameplayEffect> DamageGE = UHunterAssetManager::GetSubclass(ULyraGameData::Get().DamageGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = HunterASC->MakeOutgoingSpec(DamageGE, 1.0f, HunterASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(HunterGameplayTags::SetByCaller_Damage, DamageAmount);
		HunterASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UHunterCheatManager::HealSelf(float HealAmount)
{
	if (UHunterAbilitySystemComponent* HunterASC = GetPlayerAbilitySystemComponent())
	{
		ApplySetByCallerHeal(HunterASC, HealAmount);
	}
}

void UHunterCheatManager::HealTarget(float HealAmount)
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
		FHitResult TargetHitResult;
		AActor* TargetActor = GetTarget(HunterPC, TargetHitResult);

		if (UHunterAbilitySystemComponent* HunterTargetASC = Cast<UHunterAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor)))
		{
			ApplySetByCallerHeal(HunterTargetASC, HealAmount);
		}
	}
}

void UHunterCheatManager::ApplySetByCallerHeal(UHunterAbilitySystemComponent* HunterASC, float HealAmount)
{
	check(HunterASC);

	TSubclassOf<UGameplayEffect> HealGE = UHunterAssetManager::GetSubclass(ULyraGameData::Get().HealGameplayEffect_SetByCaller);
	FGameplayEffectSpecHandle SpecHandle = HunterASC->MakeOutgoingSpec(HealGE, 1.0f, HunterASC->MakeEffectContext());

	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(HunterGameplayTags::SetByCaller_Heal, HealAmount);
		HunterASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

UHunterAbilitySystemComponent* UHunterCheatManager::GetPlayerAbilitySystemComponent() const
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
		return HunterPC->GetHunterAbilitySystemComponent();
	}
	return nullptr;
}

void UHunterCheatManager::DamageSelfDestruct()
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
 		if (const UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(HunterPC->GetPawn()))
		{
			if (PawnExtComp->HasReachedInitState(HunterGameplayTags::InitState_GameplayReady))
			{
				if (UHunterHealthComponent* HealthComponent = UHunterHealthComponent::FindHealthComponent(HunterPC->GetPawn()))
				{
					HealthComponent->DamageSelfDestruct();
				}
			}
		}
	}
}

void UHunterCheatManager::God()
{
	if (AHunterPlayerController* HunterPC = Cast<AHunterPlayerController>(GetOuterAPlayerController()))
	{
		if (HunterPC->GetNetMode() == NM_Client)
		{
			// Automatically send cheat to server for convenience.
			HunterPC->ServerCheat(FString::Printf(TEXT("God")));
			return;
		}

		if (UHunterAbilitySystemComponent* HunterASC = HunterPC->GetHunterAbilitySystemComponent())
		{
			const FGameplayTag Tag = HunterGameplayTags::Cheat_GodMode;
			const bool bHasTag = HunterASC->HasMatchingGameplayTag(Tag);

			if (bHasTag)
			{
				HunterASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				HunterASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

void UHunterCheatManager::UnlimitedHealth(int32 Enabled)
{
	if (UHunterAbilitySystemComponent* HunterASC = GetPlayerAbilitySystemComponent())
	{
		const FGameplayTag Tag = HunterGameplayTags::Cheat_UnlimitedHealth;
		const bool bHasTag = HunterASC->HasMatchingGameplayTag(Tag);

		if ((Enabled == -1) || ((Enabled > 0) && !bHasTag) || ((Enabled == 0) && bHasTag))
		{
			if (bHasTag)
			{
				HunterASC->RemoveDynamicTagGameplayEffect(Tag);
			}
			else
			{
				HunterASC->AddDynamicTagGameplayEffect(Tag);
			}
		}
	}
}

