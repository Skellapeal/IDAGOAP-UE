using UnrealBuildTool;
using System.IO;

public class IDAGOAPThirdParty : ModuleRules
{
    public IDAGOAPThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string ThirdPartyPath = Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "IDAGOAPCore");

        // Add public include path so dependent modules can #include "rida_goap/..."
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Include"));

        // Link the static lib on Win64
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "Lib", "Win64", "IDAGOAP.lib"));
        }
    }
}
