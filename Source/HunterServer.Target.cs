// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class HunterServerTarget : TargetRules
{
	public HunterServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange(new string[] { "HunterGame" });

		HunterGameTarget.ApplySharedHunterTargetSettings(this);

		bUseChecksInShipping = true;
	}
}
