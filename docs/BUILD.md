# Build and package

## Prerequisites

The project was built on Windows with Unreal Engine **5.8.3** at `E:/UnrealEngine/UE_5.8`, Visual Studio C++ toolchain 14.50 and Windows SDK 10.0.22621. The path is a local default, not a portable requirement. No third-party runtime plugin has been installed. `PythonScriptPlugin` and `EditorScriptingUtilities` are enabled only for editor-side material authoring.

The editable project is `Unreal/KoaliticWorkbench/KoaliticWorkbench.uproject`. Open it in Unreal Editor only if editing assets interactively; normal use requires only the packaged executable.

From PowerShell at the repository root:

```powershell
& './Unreal/KoaliticWorkbench/Scripts/Build.ps1'
```

The script regenerates original WAV cues from `Scripts/GenerateUISounds.py`, builds the Editor target, authors the UI material and imports the sounds through Unreal Editor (`Scripts/PrepareAssets.py`), then runs Unreal Automation Tool to build, cook, stage and archive Win64. Its stable output is `Builds/Workbench/Windows/KoaliticWorkbench.exe`. After a successful package, it validates that executable and creates `Launch KOALITIC Workbench.lnk` at the workspace root. Shortcuts and packages are intentionally excluded from Git and are recreated locally.
Close an application running from the target package before rebuilding that package.

To use a different engine installation:

```powershell
& './Unreal/KoaliticWorkbench/Scripts/Build.ps1' -EngineRoot 'D:/Path/To/UE_5.8'
```

Build artifacts under `Binaries/`, `Intermediate/`, `Saved/` and `DerivedDataCache/` are disposable. Do not delete `Content/`, `Source/`, `Scripts/`, `Config/`, `Build/Windows/Application.ico` or the `.uproject`. Keep a packaged `Windows/` directory intact when distributing it.
The private [baseline 0.3 GitHub release](https://github.com/waylorr/KG-Unreal-Prototype-0.3/releases/tag/baseline-0.3) includes a ZIP of the earlier modular Windows package without PDB debug symbols. It is a rollback point, not the latest local build. Git tracks the source and reference media, not compiled binaries or local user presets.

## Local package cleanup

`Builds/Workbench/` is the only active output and the target of both the workspace and desktop shortcuts. The remaining `WorkbenchBeforeUnifiedNavigation/` directory is an optional rollback copy for comparing the previous interface during visual review; it can be removed after that comparison. The older `WorkbenchBeforeGroupedInspector/`, `WorkbenchSoundMilestone/`, `WorkbenchModular/` and `WorkbenchV03/` package directories were removed during cleanup. The local `KOALITIC-WorkbenchModular-Windows.zip` remains as an optional historical archive. The build script reads none of these backups. Never remove files inside the active `Workbench/Windows/` package individually.

`review/` contains disposable screenshots, logs and verification results. It can be emptied between milestones. User settings and saved presets live in `%LOCALAPPDATA%/KOALITIC/WorkbenchV03/`, outside both `Builds/` and `review/`; cleanup of these folders does not reset them.

## Verification

`-WorkbenchVerify -Evidence="absolute output directory"` runs the packaged regression harness against an isolated `VerificationKit` settings directory and writes screenshots/results to the chosen evidence folder. It checks profile state, XP event accounting, shared-link inheritance, independent entrance/exit persistence, visual-variant separation, undo/redo, progress clipping and preservation of UI Kit drafts across navigation. An earlier package completed that full harness with 23 PASS results; the current unified-navigation package received targeted packaged-app checks, including preset deletion surviving restart. Evidence under `review/` is disposable.

The opt-in `-QAControl` flag enables a local command inbox for controlled screenshots and time seeking. Neither verification mode is used by the normal shortcut. Visual quality still requires a human review of the running application.
