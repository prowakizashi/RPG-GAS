// Copyright Nebula Games

using UnrealBuildTool;
using System.Collections.Generic;

public class RPGTarget : TargetRules
{
	public RPGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "RPG" } );
	}
}
