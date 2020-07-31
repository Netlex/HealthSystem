// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// Original Author: Netlex Studio

using UnrealBuildTool;

public class HealthSystem : ModuleRules
{
	public HealthSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking" });
    }
}
