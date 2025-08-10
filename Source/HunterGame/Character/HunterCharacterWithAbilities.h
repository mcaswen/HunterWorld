// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Character/HunterCharacter.h"

#include "HunterCharacterWithAbilities.generated.h"

#define UE_API HUNTERGAME_API

class UAbilitySystemComponent;
class UHunterAbilitySystemComponent;
class UObject;

// AHunterCharacter typically gets the ability system component from the possessing player state
// This represents a character with a self-contained ability system component.
UCLASS(MinimalAPI, Blueprintable)
class AHunterCharacterWithAbilities : public AHunterCharacter
{
	GENERATED_BODY()

public:
	UE_API AHunterCharacterWithAbilities(const FObjectInitializer& ObjectInitializer);

	UE_API virtual void PostInitializeComponents() override;

	UE_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Hunter|PlayerState")
	TObjectPtr<UHunterAbilitySystemComponent> AbilitySystemComponent;
	
	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UHunterHealthSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UHunterCombatSet> CombatSet;
};

#undef UE_API
