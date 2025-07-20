// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BladeII : ModuleRules
{
	public BladeII(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"UMGEditor",
			"UnrealEd",
			"AnimGraphRuntime",
			"Sockets",
			"HTTP",
			"Networking",
			"Json",
			"JsonUtilities",
			"Paper2D",
			"MediaAssets",
			"AIModule",
			"Slate",
			"SlateCore",
			"MoviePlayer",
			"BuildPatchServices",
			"GameplayTasks",
			"EngineSettings",
			"B2GameLoadingScreen",
			"B2Network",
			"PakFile",
            "LevelSequence",
            "RenderCore",
            "NavigationSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


		PublicIncludePaths.AddRange(
			new string[]
		{
			//"CoreUObject/Public/UObject"
			"B2Network/Public",
            //"MyProject1/Text",
            //"MyProject1/Test",
        });

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"BladeII",
                "BladeII/TemplateUtility",
                "BladeII/Fulfil",
                "BladeII/Common",
                "BladeII/UI",
                "BladeII/UI/Doc",
                "BladeII/UI/Widget",
                "BladeII/UI/WidgetPart",
                "BladeII/UI/DJLegacy",
                "BladeII/UI/MsgPopup",
                "BladeII/BladeII",
                "BladeII/DevTest",
                "BladeII/InteractiveActor",
                "BladeII/InfoAsset",
                "BladeII/DataStore",
                "BladeII/Lobby",
                "BladeII/DataTrader",
                "BladeII/NetPlay",
                "BladeII/Packet",
                "B2Network",
                "BladeII/Log",
                "BladeII/Buff",
                "BladeII/AsyncPlay",
                "BladeII/AI",
                "BladeII/PassiveSkill",
                "BladeII/Misc",
                "BladeII/DLCFront",
            });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "BladeIIPCH.h"; // 或者放在 Private 也可以，只要路径正确
    }
}
