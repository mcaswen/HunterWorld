using UnrealBuildTool;
using System.Collections.Generic;

public class TestDemoEditorTarget : TargetRules
{
    public TestDemoEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        
        ExtraModuleNames.AddRange(new string[] { "TestDemo" });

        ProjectTargetShared.ApplySharedSettings(this);
    }
}
