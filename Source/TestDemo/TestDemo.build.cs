using UnrealBuildTool;

public class TestDemo : ModuleRules
{
    public TestDemo(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
				"TestDemo"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);

        PublicDependencyModuleNames.AddRange(
            new string[] { 
            "Core", 
            "CoreUObject",
            "Engine" });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
            "InputCore",
            "EnhancedInput", 
            "UMG"
        });

        DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			}
		);

        // Generate compile errors if using DrawDebug functions in test/shipping builds.
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
        
        SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
