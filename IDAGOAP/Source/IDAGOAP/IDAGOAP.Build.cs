using UnrealBuildTool;

public class IDAGOAP : ModuleRules
{
    public IDAGOAP(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AIModule",
            "NavigationSystem",
            "IDAGOAPThirdParty"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });
    }
}
