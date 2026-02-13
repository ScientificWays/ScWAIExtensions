// Scientific Ways

using UnrealBuildTool;
using System.IO; // for Path

public class ScWAIExtensions : ModuleRules
{
	public ScWAIExtensions(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{

		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",

			"AIModule",
			"NavigationSystem",
			
			"GameplayTags",
			"GameplayAbilities",

			"ModularGameplayActors",
			"ScWGameCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
		});
	}
}
