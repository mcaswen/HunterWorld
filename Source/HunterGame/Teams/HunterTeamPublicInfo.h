// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HunterTeamInfoBase.h"

#include "HunterTeamPublicInfo.generated.h"

class UHunterTeamCreationComponent;
class UHunterTeamDisplayAsset;
class UObject;
struct FFrame;

UCLASS()
class AHunterTeamPublicInfo : public AHunterTeamInfoBase
{
	GENERATED_BODY()

	friend UHunterTeamCreationComponent;

public:
	AHunterTeamPublicInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UHunterTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UHunterTeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing=OnRep_TeamDisplayAsset)
	TObjectPtr<UHunterTeamDisplayAsset> TeamDisplayAsset;
};
