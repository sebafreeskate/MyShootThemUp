// Nikita Kitov. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class MyShootThemUpTarget : TargetRules
{
	public MyShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "MyShootThemUp" } );
	}
}
