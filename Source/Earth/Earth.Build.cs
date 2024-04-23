// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Earth : ModuleRules
{
	public Earth(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","Sockets","Networking","WebSockets","Json","HeadMountedDisplay", "JsonUtilities",});
    }
}
