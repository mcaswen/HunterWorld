// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterTestsActorTestHelper.h"

#include "Character/HunterCharacter.h"
#include "Components/SkeletalMeshComponent.h"

FShooterTestsActorTestHelper::FShooterTestsActorTestHelper(APawn* Pawn)
{
	checkf(Pawn, TEXT("Pawn is invalid."));

	HunterCharacter = Cast<AHunterCharacter>(Pawn);
	checkf(HunterCharacter, TEXT("Cannot cast Pawn to a Hunter Character."));

	UActorComponent* ActorComponent = HunterCharacter->GetComponentByClass(USkeletalMeshComponent::StaticClass());
	checkf(ActorComponent, TEXT("Cannot find SkeletalMeshComponent from the HunterCharacter."));

	SkeletalMeshComponent = Cast<USkeletalMeshComponent>(ActorComponent);
	checkf(SkeletalMeshComponent, TEXT("Cannot cast component to SkeletalMeshComponent."));
	
	AbilitySystemComponent = HunterCharacter->GetLyraAbilitySystemComponent();
	checkf(AbilitySystemComponent, TEXT("Hunter Character does not have a valid AbilitySystemComponent."));

	const FName CharacterSpawn = TEXT("GameplayCue.Character.Spawn");
	GameplayCueCharacterSpawnTag = FGameplayTag::RequestGameplayTag(CharacterSpawn, true);
}

bool FShooterTestsActorTestHelper::IsPawnFullySpawned()
{
	bool bIsCurrentlySpawning = AbilitySystemComponent->IsGameplayCueActive(GameplayCueCharacterSpawnTag);
	return !bIsCurrentlySpawning;
}

FShooterTestsActorInputTestHelper::FShooterTestsActorInputTestHelper(APawn* Pawn) : FShooterTestsActorTestHelper(Pawn)
{
	PawnActions = MakeUnique<FShooterTestsPawnTestActions>(Pawn);
}

void FShooterTestsActorInputTestHelper::PerformInput(InputActionType Type)
{
	switch (Type)
	{
		case InputActionType::Crouch:
			PawnActions->ToggleCrouch();
			break;
		case InputActionType::Melee:
			PawnActions->PerformMelee();
			break;
		case InputActionType::Jump:
			PawnActions->PerformJump();
			break;
		case InputActionType::MoveForward:
			PawnActions->MoveForward();
			break;
		case InputActionType::MoveBackward:
			PawnActions->MoveBackward();
			break;
		case InputActionType::StrafeLeft:
			PawnActions->StrafeLeft();
			break;
		case InputActionType::StrafeRight:
			PawnActions->StrafeRight();
			break;
		default:
			checkNoEntry();
	}
}

void FShooterTestsActorInputTestHelper::StopAllInput()
{
	PawnActions->StopAllActions();
}