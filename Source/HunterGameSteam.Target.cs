// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HunterGameSteamTarget : HunterGameTarget
{
	public HunterGameSteamTarget(TargetInfo Target) : base(Target)
	{
		CustomConfig = "Steam";
	}
}
