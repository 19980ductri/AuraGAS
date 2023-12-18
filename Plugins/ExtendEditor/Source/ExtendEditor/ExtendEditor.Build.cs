
using UnrealBuildTool;

public class ExtendEditor : ModuleRules
{
	public ExtendEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] { }
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Source/Editor/Blutility/Private",
				//System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Plugins/Editor/EditorScriptingUtilities/Source/EditorScriptingUtilities/Private"
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"EditorScriptingUtilities",
				"Blutility", "Niagara", 
				"UMG",
				"UnrealEd",
				"AssetTools",
				"ContentBrowser"
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}