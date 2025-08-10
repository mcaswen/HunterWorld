// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "HunterPawnData.generated.h"

#define UE_API HUNTERGAME_API

class APawn;
class UHunterAbilitySet;
class UHunterAbilityTagRelationshipMapping;
class UHunterCameraMode;
class UHunterInputConfig;
class UObject;


/**
 * UHunterPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(MinimalAPI, BlueprintType, Const, Meta = (DisplayName = "Hunter Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class UHunterPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UE_API UHunterPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from AHunterPawn or AHunterCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hunter|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hunter|Abilities")
	TArray<TObjectPtr<UHunterAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hunter|Abilities")
	TObjectPtr<UHunterAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hunter|Input")
	TObjectPtr<UHunterInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hunter|Camera")
	TSubclassOf<UHunterCameraMode> DefaultCameraMode;
};

#undef UE_API
