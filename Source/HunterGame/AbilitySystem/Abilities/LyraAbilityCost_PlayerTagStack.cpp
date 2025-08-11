// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterAbilityCost_PlayerTagStack.h"

#include "GameFramework/Controller.h"
#include "LyraGameplayAbility.h"
#include "Player/HunterPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterAbilityCost_PlayerTagStack)

UHunterAbilityCost_PlayerTagStack::UHunterAbilityCost_PlayerTagStack()
{
	Quantity.SetValue(1.0f);
}

bool UHunterAbilityCost_PlayerTagStack::CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (AHunterPlayerState* PS = Cast<AHunterPlayerState>(PC->PlayerState))
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

			return PS->GetStatTagStackCount(Tag) >= NumStacks;
		}
	}
	return false;
}

void UHunterAbilityCost_PlayerTagStack::ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (AHunterPlayerState* PS = Cast<AHunterPlayerState>(PC->PlayerState))
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

				PS->RemoveStatTagStack(Tag, NumStacks);
			}
		}
	}
}

