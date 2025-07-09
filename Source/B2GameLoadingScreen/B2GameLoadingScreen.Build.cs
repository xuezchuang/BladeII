// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class B2GameLoadingScreen : ModuleRules
{
	public B2GameLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.AddRange(new string[] {
			//"../../DF/Source/B2GameLoadingScreen/Private",
			//"Public",
			//"Private",
		});

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
                "Engine",
                "MoviePlayer",
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}
