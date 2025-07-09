using System;
using System.IO;
using UnrealBuildTool;

public class B2Network : ModuleRules
{
    public B2Network(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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

		//Definitions.Add("GOOGLE_PROTOBUF_NO_RTTI=1");
		//Definitions.Add("_WINSOCK_DEPRECATED_NO_WARNINGS");
		//Definitions.Add("_CRT_SECURE_NO_WARNINGS");
		//Definitions.Add("FORCE_LOGGING_IN_ANDROID=1");

		//var thirdPartyPath = Path.GetFullPath(UEBuildConfiguration.UEThirdPartySourceDirectory);
		//PublicIncludePaths.Add(Path.Combine(thirdPartyPath, "ProtoBuf", "include"));
		//PublicIncludePaths.Add(Path.Combine(thirdPartyPath, "libsodium", "include"));
		PublicIncludePaths.AddRange(
			new string[]
		{
			"BladeII"
		});

		//string EnginePath = DirectoryReference.Combine(new DirectoryReference(Target.RelativeEnginePath), "Source").ToString();
		//string EnginePath = Path.GetFullPath(UEBuildConfiguration.UEThirdPartySourceDirectory);
		//string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty"));
		//PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "ProtoBuf", "include"));

    }
}
