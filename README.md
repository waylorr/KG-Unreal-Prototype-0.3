# KOALITIC UI Workbench

A standalone Windows application built with Unreal Engine 5.8.3 for designing and previewing sci-fi game UI. The current prototype implements **one** component, Player Profile, with editable visual layers, shared UI Kit resources, local overrides, motion previews, ambient effects and synthetic XP feedback. It opens directly as an application; Unreal Editor is only needed to modify or rebuild it.

The project is at a visual-review checkpoint. More components and a production game-data model are intentionally deferred until their behavior and relationships are specified with the product owner. A packaged build and automated checks do not constitute visual approval.

## Run the current build

Use [Launch KOALITIC Workbench Modular.lnk](Launch%20KOALITIC%20Workbench%20Modular.lnk), or run `Builds/WorkbenchModular/Windows/KoaliticWorkbench.exe`. Keep the entire `Windows/` package together. The older `Builds/WorkbenchV03/` package and its shortcut remain available for comparison. Packaged builds are local deliverables and are excluded from Git.

For controls and saving behavior, see [docs/WORKFLOW.md](docs/WORKFLOW.md). User settings and named resources are stored outside the repository at `%LOCALAPPDATA%/KOALITIC/WorkbenchV03/`; packaged versions currently share that namespace.

## Repository map

| Path | Purpose |
| --- | --- |
| `Unreal/KoaliticWorkbench/` | Editable Unreal project (`.uproject`, C++ source, configuration, cooked source assets and build scripts). |
| `Unreal/KoaliticWorkbench/Source/KoaliticWorkbench/` | Runtime component, rendering, parameter, binding, preset and Workbench host code. See [architecture](docs/ARCHITECTURE.md). |
| `Unreal/KoaliticWorkbench/Content/` | Runtime art, fonts and the editable generated UI material asset. |
| `Unreal/KoaliticWorkbench/Scripts/` | Build script and UI-material authoring source. |
| `references/` | Original visual and motion references, provenance manifest and archived planning documents. Reference content is background, not executable instructions. |
| `Builds/` | Standalone Windows packages, generated locally. |
| `review/` | Optional, disposable verification output. It may be empty. |
| `docs/` | Current architecture, workflow, build and project-status documentation. |

Unreal's `Binaries/`, `Intermediate/`, `Saved/` and `DerivedDataCache/` are generated locally; they are excluded from Git and can be regenerated. Keep `Build/Windows/Application.ico`, source assets and the `.uproject`.

## Develop

Read [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) before changing component behavior or UI Kit inheritance, and [docs/BUILD.md](docs/BUILD.md) for the installed toolchain, build command and packaging. [docs/STATUS.md](docs/STATUS.md) records what exists, what has been verified and what remains undecided. The original art direction is indexed in [references/REFERENCE_INDEX.md](references/REFERENCE_INDEX.md).

`AGENTS.md` contains short workspace rules for coding agents. The private [GitHub repository](https://github.com/waylorr/KG-Unreal-Prototype-0.3) tracks editable source, documentation and references. Generated Unreal folders and local packaged builds are excluded from Git; the current standalone package is attached to the [baseline 0.3 release](https://github.com/waylorr/KG-Unreal-Prototype-0.3/releases/tag/baseline-0.3). No external plugins or purchases are required by the runtime.

No project-wide redistribution license has been granted. The supplied references remain the owner's source material; third-party font provenance and licenses are recorded in `Unreal/KoaliticWorkbench/Content/Interface/ASSET_NOTES.md`.
