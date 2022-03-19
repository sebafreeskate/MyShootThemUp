// Nikita Kitov. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class MyShootThemUpEditorTarget : TargetRules
{
	public MyShootThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "MyShootThemUp" } );
	}
}
