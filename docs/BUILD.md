# Build and package

## Prerequisites

The project was built on Windows with Unreal Engine **5.8.3** at `E:/UnrealEngine/UE_5.8`, Visual Studio C++ toolchain 14.50 and Windows SDK 10.0.22621. The path is a local default, not a portable requirement. No third-party runtime plugin has been installed. `PythonScriptPlugin` and `EditorScriptingUtilities` are enabled only for editor-side material authoring.

The editable project is `Unreal/KoaliticWorkbench/KoaliticWorkbench.uproject`. Open it in Unreal Editor only if editing assets interactively; normal use requires only the packaged executable.

From PowerShell at the repository root:

```powershell
& './Unreal/KoaliticWorkbench/Scripts/Build.ps1'
```

The script builds the Editor target, regenerates the UI material from `Scripts/ProfileGlass.hlsl`, then runs Unreal Automation Tool to build, cook, stage and archive Win64. Its default output is `Builds/WorkbenchV03/Windows/KoaliticWorkbench.exe`. The separately packaged `Builds/WorkbenchModular/Windows/KoaliticWorkbench.exe` is the latest verified build at this documentation update; the build script does not currently target that folder.
Close an application running from the target package before rebuilding that package.

To use a different engine installation:

```powershell
& './Unreal/KoaliticWorkbench/Scripts/Build.ps1' -EngineRoot 'D:/Path/To/UE_5.8'
```

Build artifacts under `Binaries/`, `Intermediate/`, `Saved/` and `DerivedDataCache/` are disposable. Do not delete `Content/`, `Source/`, `Scripts/`, `Config/`, `Build/Windows/Application.ico` or the `.uproject`. Keep a packaged `Windows/` directory intact when distributing it.
The private [baseline 0.3 GitHub release](https://github.com/waylorr/KG-Unreal-Prototype-0.3/releases/tag/baseline-0.3) includes a ZIP of the current modular Windows package without PDB debug symbols. Git tracks the source and reference media, not compiled binaries or local user presets.

## Verification

`-WorkbenchVerify -Evidence="absolute output directory"` runs the packaged regression harness against an isolated `VerificationKit` settings directory and writes screenshots/results to the chosen evidence folder. It checks profile state, XP event accounting, shared-link inheritance, independent entrance/exit persistence, visual-variant separation, undo/redo and progress clipping. The latest modular package completed that harness with 21 PASS results on this machine. The last evidence directory was intentionally cleaned from `review/`; it can be regenerated when needed.

The opt-in `-QAControl` flag enables a local command inbox for controlled screenshots and time seeking. Neither verification mode is used by the normal shortcut. Visual quality still requires a human review of the running application.
