using UnrealBuildTool;

public class CharacterModule : ModuleRules
{
    public CharacterModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "EnhancedInput",
                "AnimGraphRuntime",
                "GameplayTasks",
                "UMG",
                "MotionWarping",
                "Niagara",
                "NavigationSystem",
                "InventoryModule",
                "GameplayAbilities",
                "GameplayTags",
                "AIModule",
                "CoreModule",
            }
        );
    }
}