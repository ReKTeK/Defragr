// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DefragrEditorTarget : TargetRules
{
	public DefragrEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange( new string[] { "Defragr" } );
	}
}
