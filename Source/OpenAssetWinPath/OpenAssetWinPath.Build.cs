namespace UnrealBuildTool.Rules
{
	public class OpenAssetWinPath : ModuleRules
	{
		public OpenAssetWinPath(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"UnrealEd",
					"LevelEditor",
					"Slate",
					"SlateCore",
					"MainFrame",
					"InputCore",
					"CoreUObject",
					"EditorStyle",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
				}
			);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}
