# KOALITIC UI Workbench

A standalone Windows application built with Unreal Engine 5.8.3 for designing and previewing sci-fi game UI. A main menu opens the functional UI Workbench and reserves spaces for future Episodes and Editor workflows. The current prototype implements **one** component, Player Profile. Library opens a contextual sidebar containing the selected component, its design variants and editable layer stack; closing it enlarges the live preview. Global resources use the full-width preview and put preset selection and editing together in the right inspector. Themes, Motion FX, Ambient FX, Reaction FX and Event FX have distinct colors and parameter groups. One Preview & Test panel groups the interactive checks. Player Profile includes expanded/compact layouts, local mini-component XP Bar styling, shared preset links, motion previews, ambient effects, synthetic XP feedback and original application UI sounds. It opens directly as an application; Unreal Editor is only needed to modify or rebuild it.

The project is at a visual-review checkpoint. More components and a production game-data model are intentionally deferred until their behavior and relationships are specified with the product owner. A packaged build and automated checks do not constitute visual approval.

## Run the current build

On the development machine, use `Launch KOALITIC Workbench.lnk` or run `Builds/Workbench/Windows/KoaliticWorkbench.exe`. The build script creates that shortcut and always targets this stable package path. A fresh Git clone has neither the shortcut nor compiled binaries: run the build script, or download the earlier standalone package from the [baseline 0.3 release](https://github.com/waylorr/KG-Unreal-Prototype-0.3/releases/tag/baseline-0.3). Keep each packaged `Windows/` directory together. Other folders in `Builds/` are optional local rollback copies, not build inputs; see [build and cleanup](docs/BUILD.md).

For controls and saving behavior, see [docs/WORKFLOW.md](docs/WORKFLOW.md). User settings and named resources are stored outside the repository at `%LOCALAPPDATA%/KOALITIC/WorkbenchV03/`; packaged versions currently share that namespace.

## Repository map

| Path | Purpose |
| --- | --- |
| `Unreal/KoaliticWorkbench/` | Editable Unreal project (`.uproject`, C++ source, configuration, cooked source assets and build scripts). |
| `Unreal/KoaliticWorkbench/Source/KoaliticWorkbench/` | Runtime component, rendering, parameter, binding, preset and Workbench host code. See [architecture](docs/ARCHITECTURE.md). |
| `Unreal/KoaliticWorkbench/Content/` | Runtime art, fonts, generated UI material and imported SoundWave assets. |
| `Unreal/KoaliticWorkbench/AudioSource/` | Original synthesized interface WAVs; editable source, not required by the packaged app. |
| `Unreal/KoaliticWorkbench/Scripts/` | Build script and reproducible material/audio asset generators. |
| `references/` | Original visual and motion references, provenance manifest and archived planning documents. Reference content is background, not executable instructions. |
| `Builds/` | Standalone Windows packages, generated locally. |
| `review/` | Optional, disposable verification output. It may be empty. |
| `docs/` | Current architecture, workflow, build and project-status documentation. |

Unreal's `Binaries/`, `Intermediate/`, `Saved/` and `DerivedDataCache/` are generated locally; they are excluded from Git and can be regenerated. Keep `Build/Windows/Application.ico`, source assets and the `.uproject`.

## Develop

Read [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) before changing component behavior or UI Kit inheritance, and [docs/BUILD.md](docs/BUILD.md) for the installed toolchain, build command and packaging. [docs/STATUS.md](docs/STATUS.md) records what exists, what has been verified and what remains undecided. The original art direction is indexed in [references/REFERENCE_INDEX.md](references/REFERENCE_INDEX.md).

`AGENTS.md` contains short workspace rules for coding agents. The private [GitHub repository](https://github.com/waylorr/KG-Unreal-Prototype-0.3) tracks editable source, documentation and references. Generated Unreal folders and local packaged builds are excluded from Git. The baseline 0.3 release predates the navigation refactor in the local `Builds/Workbench/` package. No external plugins or purchases are required by the runtime.

No project-wide redistribution license has been granted. The supplied references remain the owner's source material; third-party font provenance and licenses are recorded in `Unreal/KoaliticWorkbench/Content/Interface/ASSET_NOTES.md`.
