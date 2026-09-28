# Architecture

## Runtime boundary

`Unreal/KoaliticWorkbench` is a native Unreal Engine 5.8 C++ project. Its game mode installs `SWorkbench`, a Slate widget, into the game viewport. The packaged application does not host Unreal Editor. `SWorkbench` owns the editor shell, interaction and preview session; the Player Profile drawing code does not depend on that shell.

| Source file | Responsibility |
| --- | --- |
| `WorkbenchGameMode.h`, `Workbench.cpp` | Startup, Slate host, input dispatch, base profile persistence, preview events and verification entry points. |
| `WorkbenchView.inl` | Workbench layout: Components, UI Kit, central preview, inspector and test controls. Included inside `SWorkbench`. |
| `WorkbenchKit.inl` | UI Kit orchestration, draft selection, undo/redo and UI-material parameter updates. Included inside `SWorkbench`. |
| `WorkbenchCanvas.h` | Low-level Slate drawing, clipping, typography and colors. |
| `PlayerProfile.h` | Profile data, presentation and state types; clock-driven evaluation and layered component renderer. |
| `ProgressPart.h` | Data-agnostic progress-track and fill geometry. It receives normalized progress; it owns no XP rules. |
| `KitParameters.h` | Registry of the 36 editable values, categories, ranges and typed accessors for the current presentation. |
| `KitBindings.h` | Per-value inheritance and local override resolution, plus component binding serialization. |
| `KitPresetStore.h` | Named UI Kit resource listing, loading, saving and default seeding in `UIKit.ini`. It does not draw UI. |
| `KitParameterEditor.h` | One parameter-control renderer used by both the global UI Kit and component inspector. |
| `KitVerification.inl` | Opt-in packaged regression checks and screenshot capture. |

The two inspector scopes share the parameter registry and control renderer. UI Kit edits a category draft and stores a named resource; Components edits the Player Profile's local presentation. Each parameter either follows its linked shared value or has a local override. Browsing a resource auditions it without changing the assignment. Applying a saved resource changes that category's link but preserves local overrides. Saving the component persists links and overrides separately from saving the UI Kit resource.

## Rendering and time

The profile is not a flattened reference screenshot. Frame, glass, text, progress, lighting and overlays are drawn as distinct native layers; portrait artwork is sampled from the supplied references. `M_ProfileGlass` is a UI-domain material driven by the same preview clock as Slate animation. Its editable authoring source is `Scripts/ProfileGlass.hlsl`; `Scripts/CreateInterfaceMaterial.py` regenerates the Unreal material asset during a full build. UI glow and procedural particles are composed in the interface renderer, not through scene bloom or Niagara.

`ProfileData` is the base fixture. `Presentation` contains visual and motion settings. `ProfileState` controls layout and interaction. `Playback` supplies presentation time. Synthetic rewards are timestamped preview events evaluated against the fixture; visual replay does not award XP. The progress part takes the resulting normalized value and can later be fed by a different data source.

## Persistence and current limits

`%LOCALAPPDATA%/KOALITIC/WorkbenchV03/Presentation.ini` stores the base profile, links and local values; `UIKit.ini` stores shared category resources. Preview rewards, pause time and unsaved drafts are not persisted. Legacy `Style` and `Motion` keys can still be read as a migration fallback; new saves use the registered local-value schema. The old `MotionPresets.ini` route is inactive and its file is not deleted.

The code is more modular than the first prototype, but it is **not yet a generic multi-component framework**: `KitParameters.h` and `Presentation` describe Player Profile, and `SWorkbench` still coordinates much of the screen and session. Before adding another component, define its data contract, reusable parts and category/binding needs. Do not clone the Player Profile or invent a universal schema prematurely.
