using System.IO;
using UnrealBuildTool;

public class ZpeopleUEUtilites : ModuleRules
{
	public ZpeopleUEUtilites(ReadOnlyTargetRules Target) : base(Target)
	{


        PrivateIncludePaths.AddRange(
            new string[] {
                "ZpeopleUEUtilites/Private",
            });
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core","CoreUObject","Engine","InputCore"
            });
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
				"Projects",
				"InputCore",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "HTTP",
                "Networking",
                "ImageWrapper",
                "Json"
            });
	}
}
