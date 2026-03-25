// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FinalYearProject : ModuleRules
{
	public FinalYearProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AIModule",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"IDAGOAP",
			"IDAGOAPThirdParty"
		});
	}
}
