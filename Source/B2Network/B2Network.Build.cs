using System;
using System.IO;
using UnrealBuildTool;

public class B2Network : ModuleRules
{
    public B2Network(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
             new string[] {
                 "Core",
                 "CoreUObject"
             }
         );

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "BladeII",
                "BladeII/BladeII",
                "B2Network",
                "B2Network/Public",
            });

        bEnableExceptions = true; // for enabling c++ exception

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            // Allow exception only for editor
            //PublicDefinitions.Add("B2NETWORK_PLATFORM_EXCEPTION=1");
        }

        
        var ProtobufIncludePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", "ThirdParty", "ProtoBuf", "include"));
        PublicIncludePaths.Add(ProtobufIncludePath);
        //_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "B2NetworkPCH.h"; // 或者放在 Private 也可以，只要路径正确


        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("B2NETWORK_PLATFORM_WINDOWS=1");
            PublicDefinitions.Add("SODIUM_STATIC=1");
            PublicDefinitions.Add("SODIUM_EXPORT= ");
            PublicDefinitions.Add("_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS");
            

            //PublicAdditionalCompilerOptions.Add("/D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING");
            //PublicSystemLibraryPaths.Add(Path.Combine(ThirdPartyPath, "ProtoBuf", "lib", "Windows", "x64"));
            //PublicSystemLibraryPaths.Add(Path.Combine(ThirdPartyPath, "libsodium", "lib", "Windows", "x64"));

            //PublicAdditionalLibraries.Add("libprotobuf.lib");
            //PublicAdditionalLibraries.Add("libsodium.lib");
            // 
            string ProtobufPath = Path.Combine(ModuleDirectory, "..", "ThirdParty", "ProtoBuf");
            PublicAdditionalLibraries.Add(Path.Combine(ProtobufPath, "lib", "Windows", "x64", "libprotobuf.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            //PublicDefinitions.Add("B2NETWORK_PLATFORM_ANDROID=1");

            //var protobuf_libPath_arm64 = Path.Combine(thirdPartyPath, "ProtoBuf", "lib", "Android", "arm64");
            //var protobuf_libPath_armv7 = Path.Combine(thirdPartyPath, "ProtoBuf", "lib", "Android", "armv7");
            //var sodium_libPath_arm64 = Path.Combine(thirdPartyPath, "libsodium", "lib", "Android", "arm64");
            //var sodium_libPath_armv7 = Path.Combine(thirdPartyPath, "libsodium", "lib", "Android", "armv7");

            //PublicLibraryPaths.Add(protobuf_libPath_arm64);
            //PublicLibraryPaths.Add(protobuf_libPath_armv7);
            //PublicLibraryPaths.Add(sodium_libPath_arm64);
            //PublicLibraryPaths.Add(sodium_libPath_armv7);

            //PublicAdditionalLibraries.Add("protobuf");
            //PublicAdditionalLibraries.Add("sodium");
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            //PublicDefinitions.Add("B2NETWORK_PLATFORM_IOS=1");

            //var protobuf_libPath = Path.Combine(thirdPartyPath, "ProtoBuf", "lib", "iOS");
            //var sodium_libPath = Path.Combine(thirdPartyPath, "libsodium", "lib", "iOS");

            //PublicLibraryPaths.Add(protobuf_libPath);
            //PublicLibraryPaths.Add(sodium_libPath);

            //PublicAdditionalLibraries.Add(Path.Combine(protobuf_libPath, "libprotobuf_arm64.a"));
            //PublicAdditionalLibraries.Add(Path.Combine(sodium_libPath, "libsodium.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            //PublicDefinitions.Add("B2NETWORK_PLATFORM_IOS=1");

            //var protobuf_libPath = Path.Combine(thirdPartyPath, "ProtoBuf", "lib", "iOS");
            //var sodium_libPath = Path.Combine(thirdPartyPath, "libsodium", "lib", "iOS");

            //PublicLibraryPaths.Add(protobuf_libPath);
            //PublicLibraryPaths.Add(sodium_libPath);

            //PublicAdditionalLibraries.Add(Path.Combine(protobuf_libPath, "libprotobuf.a"));
            //PublicAdditionalLibraries.Add(Path.Combine(sodium_libPath, "libsodium.a"));
        }
        else
        {
            throw new Exception("Platform not supported");
        }
    }
}
