// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HunterTeamInfoBase.h"

#include "HunterTeamPrivateInfo.generated.h"

class UObject;

UCLASS()
class AHunterTeamPrivateInfo : public AHunterTeamInfoBase
{
	GENERATED_BODY()

public:
	AHunterTeamPrivateInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
