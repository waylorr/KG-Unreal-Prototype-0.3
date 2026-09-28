using UnrealBuildTool;
public class KoaliticWorkbench : ModuleRules { public KoaliticWorkbench(ReadOnlyTargetRules Target) : base(Target) { PCHUsage=PCHUsageMode.UseExplicitOrSharedPCHs; PublicDependencyModuleNames.AddRange(new string[]{"Core","CoreUObject","ApplicationCore","Engine","InputCore","Slate","SlateCore","ImageWrapper","RenderCore","RHI"}); } }

