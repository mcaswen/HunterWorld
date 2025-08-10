// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HunterClientTarget : TargetRules
{
	public HunterClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange(new string[] { "HunterGame" });

		HunterGameTarget.ApplySharedHunterTargetSettings(this);
	}
}
