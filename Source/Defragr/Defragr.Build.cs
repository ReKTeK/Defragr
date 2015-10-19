// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Defragr : ModuleRules
{
	public Defragr(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"Slate",
			"SlateCore",
			"Blu",
			"Json",
			"JsonUtilities"
		});

		PublicIncludePaths.AddRange(new string[] { "Blu/Public" });
	}
}
