using UnrealBuildTool;
using System.IO;

public class IDAGOAPThirdParty : ModuleRules
{
    public IDAGOAPThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string ThirdPartyPath = Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "IDAGOAPCore");

        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Include"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "Lib", "Win64", "IDAGOAP.lib"));
        }
    }
}