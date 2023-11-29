//Unreal® Engine, Copyright 1998 – 2023, Epic Games, Inc. All rights reserved.
using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class PICOXRDPHMD : ModuleRules
	{
		public PICOXRDPHMD(ReadOnlyTargetRules Target) : base(Target)
		{
			string EnginePath = Path.GetFullPath(Target.RelativeEnginePath);
			PrivateIncludePaths.AddRange(
				new string[] {
					"PICOXRDPHMD/Private",
					"PICOXRDPHMD/Public",
					EnginePath+"Source/Runtime/Renderer/Private",
					// ... add other private include paths required here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"RHI",
					"RenderCore",
					"Renderer",
                    "InputCore",
					"InputDevice",
					"HeadMountedDisplay",
					"Slate",
					"SlateCore",
					"RHICore"
				}
				);
            
            if (Target.bBuildEditor == true)
            {
                PrivateDependencyModuleNames.Add("UnrealEd");
            }

			if (Target.Platform == UnrealTargetPlatform.Win64 || Target.IsInPlatformGroup(UnrealPlatformGroup.Linux))
			{
				AddEngineThirdPartyPrivateStaticDependencies(Target, "NVAftermath");
			}

			if (Target.Platform == UnrealTargetPlatform.Win64)
            {
				PrivateDependencyModuleNames.AddRange(
					new string[]
					{
							"D3D11RHI",
							"D3D12RHI",
					});

				PrivateIncludePaths.AddRange(
					new string[]
					{
							EnginePath+"Source/Runtime/Windows/D3D11RHI/Private",
							EnginePath+"Source/Runtime/Windows/D3D11RHI/Private/Windows",
							EnginePath+"Source/Runtime/D3D12RHI/Private",
							EnginePath+"Source/Runtime/D3D12RHI/Private/Windows",
					});

				AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11");
				AddEngineThirdPartyPrivateStaticDependencies(Target, "DX12");
				AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenVR");
                AddEngineThirdPartyPrivateStaticDependencies(Target, "NVAftermath");
                AddEngineThirdPartyPrivateStaticDependencies(Target, "IntelMetricsDiscovery");
				AddEngineThirdPartyPrivateStaticDependencies(Target, "IntelExtensionsFramework");

                string PICOXRLibsDirectory = Path.Combine(ModuleDirectory, "../ThirdParty/Libs");

				PublicIncludePaths.Add(Path.Combine(PICOXRLibsDirectory, "Include"));

				PublicAdditionalLibraries.Add(Path.Combine(PICOXRLibsDirectory, "Include/streamingDriverInterface", "pxr_driver_sdk.lib"));
				PublicAdditionalLibraries.Add(Path.Combine(PICOXRLibsDirectory, "Include/streamingDriverInterface", "pxr_base.lib"));

				PublicDelayLoadDLLs.Add("pxr_driver_sdk.dll");
				PublicDelayLoadDLLs.Add("pxr_base.dll");
				PublicDelayLoadDLLs.Add("pxr_rpc.dll");

				RuntimeDependencies.Add(Path.Combine(PICOXRLibsDirectory, "Include/streamingDriverInterface", "pxr_rpc.dll"));
				RuntimeDependencies.Add(Path.Combine(PICOXRLibsDirectory, "Include/streamingDriverInterface", "pxr_driver_sdk.dll"));
				RuntimeDependencies.Add(Path.Combine(PICOXRLibsDirectory, "Include/streamingDriverInterface", "pxr_base.dll"));

			}
		}
	}
}
