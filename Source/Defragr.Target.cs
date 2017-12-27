// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DefragrTarget : TargetRules
{
	public DefragrTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "Defragr" } );
	}
}
