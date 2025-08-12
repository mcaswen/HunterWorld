using UnrealBuildTool;
using Microsoft.Extensions.Logging;
using System.Collections.Generic;
using EpicGames.Core;
using System.IO;
using UnrealBuildBase;

public class HunterWorldTarget : TargetRules
{
    public HunterWorldTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;

        ExtraModuleNames.AddRange(new string[] { "HunterWorld" });

        ProjectTargetShared.ApplySharedSettings(this);
    }
}

internal static class ProjectTargetShared
{
    internal static void ApplySharedSettings(TargetRules Target)
    {
        ILogger Logger = Target.Logger;
        Target.DefaultBuildSettings = BuildSettingsVersion.V5;
        Target.IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        bool bIsTest = Target.Configuration == UnrealTargetConfiguration.Test;
        bool bIsShipping = Target.Configuration == UnrealTargetConfiguration.Shipping;
        bool bIsDedicatedServer = Target.Type == TargetType.Server;

        if (Target.BuildEnvironment == TargetBuildEnvironment.Unique)
        {
            Target.CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Error;
            Target.bUseLoggingInShipping = true;
            Target.bTrackRHIResourceInfoForTest = true;

            if (bIsShipping && !bIsDedicatedServer)
                Target.bDisableUnverifiedCertificates = true;

            if (bIsShipping || bIsTest)
            {
                Target.bAllowGeneratedIniWhenCooked = false;
                Target.bAllowNonUFSIniWhenCooked = false;
            }

            if (Target.Type != TargetType.Editor)
            {
                Target.DisablePlugins.Add("OpenImageDenoise");
                Target.GlobalDefinitions.Add("UE_ASSETREGISTRY_INDIRECT_ASSETDATA_POINTERS=1");
            }

            ConfigureGameFeaturePlugins(Target);
        }
        else
        {
            if (Target.Type == TargetType.Editor)
                ConfigureGameFeaturePlugins(Target);
            else
                Target.Logger.LogWarning("Dynamic plugin toggles are disabled with installed engine (shared).");
        }
    }

    internal static void ConfigureGameFeaturePlugins(TargetRules Target)
    {
        bool bBuildAll = false;
        var list = new List<FileReference>();
        foreach (var dir in Unreal.GetExtensionDirs(Target.ProjectFile.Directory, Path.Combine("Plugins", "GameFeatures")))
            list.AddRange(PluginsBase.EnumeratePlugins(dir));

        foreach (var pluginFile in list)
        {
            bool enable = bBuildAll, forceDisable = false;
            try
            {
                if (FileReference.Exists(pluginFile))
                {
                    var raw = JsonObject.Read(pluginFile);

                    bool explicitlyLoaded = false;
                    if (!raw.TryGetBoolField("ExplicitlyLoaded", out explicitlyLoaded) || !explicitlyLoaded)
                        Target.Logger.LogWarning("GameFeaturePlugin {0} should set ExplicitlyLoaded=true.", pluginFile.GetFileNameWithoutExtension());

                    bool editorOnly = false;
                    if (raw.TryGetBoolField("EditorOnly", out editorOnly))
                        if (editorOnly && Target.Type != TargetType.Editor && !bBuildAll) forceDisable = true;

                    string restrict;
                    if (raw.TryGetStringField("RestrictToBranch", out restrict))
                        if (!Target.Version.BranchName.Equals(restrict, System.StringComparison.OrdinalIgnoreCase)) forceDisable = true;

                    bool neverBuild = false;
                    if (raw.TryGetBoolField("NeverBuild", out neverBuild) && neverBuild) forceDisable = true;
                }
            }
            catch
            {
                forceDisable = true;
            }

            if (forceDisable) enable = false;
            if (enable) Target.EnablePlugins.Add(pluginFile.GetFileNameWithoutExtension());
            else if (forceDisable) Target.DisablePlugins.Add(pluginFile.GetFileNameWithoutExtension());
        }
    }
}

