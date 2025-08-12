using UnrealBuildTool;
using System.Collections.Generic;

public class TestDemoTarget : TargetRules
{
    public TestDemoTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;

        ExtraModuleNames.AddRange(new string[] { "TestDemo" });

        ProjectTargetShared.ApplySharedSettings(this);
    }
}