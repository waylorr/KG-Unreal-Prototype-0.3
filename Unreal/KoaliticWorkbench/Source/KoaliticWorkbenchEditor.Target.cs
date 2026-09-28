using UnrealBuildTool;
public class KoaliticWorkbenchEditorTarget : TargetRules { public KoaliticWorkbenchEditorTarget(TargetInfo Target) : base(Target) { Type=TargetType.Editor; DefaultBuildSettings=BuildSettingsVersion.V7; IncludeOrderVersion=EngineIncludeOrderVersion.Latest; ExtraModuleNames.Add("KoaliticWorkbench"); } }

