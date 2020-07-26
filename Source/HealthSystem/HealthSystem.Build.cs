// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/Netlex/HealthSystem
// Original Author: Netlex Studio

using UnrealBuildTool;

public class HealthSystem : ModuleRules
{
	public HealthSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
