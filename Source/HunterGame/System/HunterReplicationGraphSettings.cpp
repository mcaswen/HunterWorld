// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterReplicationGraphSettings.h"
#include "Misc/App.h"
#include "System/HunterReplicationGraph.h"

UHunterReplicationGraphSettings::UHunterReplicationGraphSettings()
{
	CategoryName = TEXT("Game");
	DefaultReplicationGraphClass = UHunterReplicationGraph::StaticClass();
}