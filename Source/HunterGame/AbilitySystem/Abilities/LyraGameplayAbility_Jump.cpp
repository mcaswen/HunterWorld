// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraGameplayAbility_Jump.h"

#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Character/HunterCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameplayAbility_Jump)

struct FGameplayTagContainer;


ULyraGameplayAbility_Jump::ULyraGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool ULyraGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const AHunterCharacter* HunterCharacter = Cast<AHunterCharacter>(ActorInfo->AvatarActor.Get());
	if (!HunterCharacter || !HunterCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void ULyraGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULyraGameplayAbility_Jump::CharacterJumpStart()
{
	if (AHunterCharacter* HunterCharacter = GetHunterCharacterFromActorInfo())
	{
		if (HunterCharacter->IsLocallyControlled() && !HunterCharacter->bPressedJump)
		{
			HunterCharacter->UnCrouch();
			HunterCharacter->Jump();
		}
	}
}

void ULyraGameplayAbility_Jump::CharacterJumpStop()
{
	if (AHunterCharacter* HunterCharacter = GetHunterCharacterFromActorInfo())
	{
		if (HunterCharacter->IsLocallyControlled() && HunterCharacter->bPressedJump)
		{
			HunterCharacter->StopJumping();
		}
	}
}

