// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CrashEditorTarget : TargetRules
{
	public CrashEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        	DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("Crash");
	}
}
