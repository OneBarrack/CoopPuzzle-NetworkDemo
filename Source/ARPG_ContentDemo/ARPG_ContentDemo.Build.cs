// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ARPG_ContentDemo : ModuleRules
{
	public ARPG_ContentDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
