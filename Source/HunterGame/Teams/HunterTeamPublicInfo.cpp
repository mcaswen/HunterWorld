// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterTeamPublicInfo.h"

#include "Net/UnrealNetwork.h"
#include "Teams/HunterTeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterTeamPublicInfo)

class FLifetimeProperty;

AHunterTeamPublicInfo::AHunterTeamPublicInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AHunterTeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void AHunterTeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UHunterTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void AHunterTeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}

