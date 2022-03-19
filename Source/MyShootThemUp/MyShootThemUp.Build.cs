// Nikita Kitov. All rights reserved

using UnrealBuildTool;

public class MyShootThemUp : ModuleRules
{
	public MyShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core"
            ,"CoreUObject"
            ,"Engine"
            ,"InputCore"
            ,"Niagara"
            ,"PhysicsCore"
            ,"GameplayTasks"
            ,"NavigationSystem"
            });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] { 
            "MyShootThemUp/Player"
            ,"MyShootThemUp/Components/Weapon"
            ,"MyShootThemUp/Components"
            ,"MyShootThemUp/Dev"
            ,"MyShootThemUp/HUD"
            ,"MyShootThemUp/GameFramework"
            ,"MyShootThemUp/Weapon"
            ,"MyShootThemUp/AnimNotifies"
            ,"MyShootThemUp/CoreTypes"
            ,"MyShootThemUp/Utils"
            ,"MyShootThemUp/Pickups"
            ,"MyShootThemUp/AI"
            });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
