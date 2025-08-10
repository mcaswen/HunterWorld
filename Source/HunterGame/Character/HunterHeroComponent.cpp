// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterHeroComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "HunterLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Player/HunterPlayerController.h"
#include "Player/HunterPlayerState.h"
#include "Player/HunterLocalPlayer.h"
#include "Character/HunterPawnExtensionComponent.h"
#include "Character/HunterPawnData.h"
#include "Character/HunterCharacter.h"
#include "AbilitySystem/HunterAbilitySystemComponent.h"
#include "Input/HunterInputConfig.h"
#include "Input/HunterInputComponent.h"
#include "Camera/HunterCameraComponent.h"
#include "HunterGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/HunterCameraMode.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterHeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace HunterHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UHunterHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UHunterHeroComponent::NAME_ActorFeatureName("Hero");

UHunterHeroComponent::UHunterHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void UHunterHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogHunter, Error, TEXT("[UHunterHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("HunterHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("HunterHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

bool UHunterHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == HunterGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == HunterGameplayTags::InitState_Spawned && DesiredState == HunterGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AHunterPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AHunterPlayerController* HunterPC = GetController<AHunterPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !HunterPC || !HunterPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == HunterGameplayTags::InitState_DataAvailable && DesiredState == HunterGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AHunterPlayerState* HunterPS = GetPlayerState<AHunterPlayerState>();

		return HunterPS && Manager->HasFeatureReachedInitState(Pawn, UHunterPawnExtensionComponent::NAME_ActorFeatureName, HunterGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == HunterGameplayTags::InitState_DataInitialized && DesiredState == HunterGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UHunterHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == HunterGameplayTags::InitState_DataAvailable && DesiredState == HunterGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AHunterPlayerState* HunterPS = GetPlayerState<AHunterPlayerState>();
		if (!ensure(Pawn && HunterPS))
		{
			return;
		}

		const UHunterPawnData* PawnData = nullptr;

		if (UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UHunterPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(HunterPS->GetHunterAbilitySystemComponent(), HunterPS);
		}

		if (AHunterPlayerController* HunterPC = GetController<AHunterPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UHunterCameraComponent* CameraComponent = UHunterCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UHunterHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UHunterPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == HunterGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UHunterHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { HunterGameplayTags::InitState_Spawned, HunterGameplayTags::InitState_DataAvailable, HunterGameplayTags::InitState_DataInitialized, HunterGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UHunterHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UHunterPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(HunterGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UHunterHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UHunterHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UHunterLocalPlayer* LP = Cast<UHunterLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UHunterPawnData* PawnData = PawnExtComp->GetPawnData<UHunterPawnData>())
		{
			if (const UHunterInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.LoadSynchronous())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}
							
							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The Hunter Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UHunterInputComponent or modify this component accordingly.
				UHunterInputComponent* HunterIC = Cast<UHunterInputComponent>(PlayerInputComponent);
				if (ensureMsgf(HunterIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UHunterInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					HunterIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					HunterIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					HunterIC->BindNativeAction(InputConfig, HunterGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					HunterIC->BindNativeAction(InputConfig, HunterGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					HunterIC->BindNativeAction(InputConfig, HunterGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					HunterIC->BindNativeAction(InputConfig, HunterGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					HunterIC->BindNativeAction(InputConfig, HunterGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UHunterHeroComponent::AddAdditionalInputConfig(const UHunterInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UHunterInputComponent* HunterIC = Pawn->FindComponentByClass<UHunterInputComponent>();
		if (ensureMsgf(HunterIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UHunterInputComponent or a subclass of it.")))
		{
			HunterIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UHunterHeroComponent::RemoveAdditionalInputConfig(const UHunterInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UHunterHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UHunterHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UHunterAbilitySystemComponent* HunterASC = PawnExtComp->GetHunterAbilitySystemComponent())
			{
				HunterASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UHunterHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UHunterAbilitySystemComponent* HunterASC = PawnExtComp->GetHunterAbilitySystemComponent())
		{
			HunterASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UHunterHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (AHunterPlayerController* HunterController = Cast<AHunterPlayerController>(Controller))
	{
		HunterController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UHunterHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UHunterHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * HunterHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * HunterHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UHunterHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AHunterCharacter* Character = GetPawn<AHunterCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UHunterHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (AHunterPlayerController* Controller = Cast<AHunterPlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}	
	}
}

TSubclassOf<UHunterCameraMode> UHunterHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UHunterPawnExtensionComponent* PawnExtComp = UHunterPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UHunterPawnData* PawnData = PawnExtComp->GetPawnData<UHunterPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void UHunterHeroComponent::SetAbilityCameraMode(TSubclassOf<UHunterCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UHunterHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

