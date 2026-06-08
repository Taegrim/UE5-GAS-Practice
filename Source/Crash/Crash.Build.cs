// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Crash : ModuleRules
{
	public Crash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
