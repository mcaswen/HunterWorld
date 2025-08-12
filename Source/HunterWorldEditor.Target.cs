using UnrealBuildTool;
using System.Collections.Generic;

public class HunterWorldEditorTarget : TargetRules
{
    public HunterWorldEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "HunterWorld" });

        ProjectTargetShared.ApplySharedSettings(this);
    }
}
