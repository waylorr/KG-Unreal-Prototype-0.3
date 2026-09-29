# Architecture

## Runtime boundary

`Unreal/KoaliticWorkbench` is one native Unreal Engine 5.8 C++ project for the application. Its game mode installs `SWorkbench`, a Slate widget, into the game viewport. The packaged application does not host Unreal Editor. A lightweight top-level shell navigates between Home, UI Workbench and reserved Episodes/Editor screens; those future workspaces do not contain asset or timeline logic yet. `SWorkbench` owns the interaction and preview session; the Player Profile drawing code does not depend on the shell.

| Source file | Responsibility |
| --- | --- |
| `WorkbenchGameMode.h`, `Workbench.cpp` | Startup, Slate host, input collection, base profile persistence, preview events and verification entry points. |
| `WorkbenchView.inl` | Primary navigation, contextual second sidebar, responsive live preview, selection-based inspector and Preview & Test panel. Included inside `SWorkbench`. |
| `WorkbenchShell.inl` | Main menu, reserved workspace screens and shared preset-picker overlay. Included inside `SWorkbench`. |
| `WorkbenchKit.inl` | UI Kit orchestration, draft selection, undo/redo and UI-material parameter updates. Included inside `SWorkbench`. |
| `WorkbenchActions.h`, `WorkbenchActions.inl` | Named hit-test IDs and one action route shared by mouse and keyboard. The `.inl` contains application-side effects and is included inside `SWorkbench`. |
| `WorkbenchSound.h` | Maps semantic actions to six application cue families without coupling audio to the preview clock or component data. |
| `WorkbenchNavigation.h` | Visible sidebar section, internal component/resource scope, independent selected categories, inspector tab and scroll positions. It owns no component data or rendering. |
| `WorkbenchCanvas.h` | Low-level Slate drawing, clipping, typography and colors. |
| `WorkbenchSkin.h` | App-only beveled panels, glass header bands, control wells, icon glyphs and preview-grid primitives. Kept separate from the Player Profile renderer and shared resource data. |
| `PlayerProfile.h` | Profile data, presentation and state types; clock-driven evaluation and layered component renderer. |
| `ProfileVariants.h` | Saves complete Player Profile visual variants, including layers, portrait, resource links and local values; it does not duplicate profile data. |
| `ProgressPart.h` | Data-agnostic progress-track and fill geometry. It receives normalized progress; it owns no XP rules. |
| `KitParameters.h` | Registry of 86 editable values, categories, ranges and typed accessors for the current presentation. |
| `KitBindings.h` | Per-value inheritance and local override resolution, plus component binding serialization. |
| `KitPresetStore.h` | Named UI Kit resource listing, loading, saving and default seeding in `UIKit.ini`. It does not draw UI. |
| `KitParameterEditor.h`, `KitParameterGroups.h` | One clipped, scrollable parameter-control renderer and category/group metadata used by both the global UI Kit and component inspector. |
| `KitVerification.inl` | Opt-in packaged regression checks and screenshot capture. |

The two inspector scopes share the parameter registry and control renderer. Global resource browsing and draft editing take place together in the right inspector while the central preview expands. The component inspector uses Data & links for base data and resource assignments, and Design overrides for values local to this design. The Library sidebar contains the selected component's variants and layers; selecting a layer, including the local XP Bar, shows contextual controls in the right panel. Each shared parameter either follows its linked value or has a local override. The XP Bar drawing parameters remain local to the design and are excluded from global navigation and shared preset seeding; its fill color can follow a Theme and be locally overridden. Themes, Motion, Ambient, Reaction and Event resources use separate categories, with independent Entrance/Exit and Pulse/XP-event behavior. Browsing shared presets caches unsaved drafts per preset; choosing one previews its values immediately but leaves the component's saved link unchanged. Choosing a linked preset in Data & links changes the active variant's link in memory and marks the component unsaved. Global drafts do not affect linked designs until Save. Saving the component persists its data, variants, links and overrides separately from saving a shared resource.

**Vocabulary:** a *component* such as Player Profile presents data and behavior; its *design* is a visual composition with variants and a stack of editable layers; XP Bar is a reusable mini component placed in that design, accepting normalized progress rather than owning XP. Future left/top/right HUD bars are containers that position components, not owners of their data. Resource preset deletion is blocked while any saved or active design links to the preset; saved presets are not automatically reseeded after deletion.

## Rendering and time

The profile is not a flattened reference screenshot. Frame, glass, text, progress, lighting and overlays are drawn as distinct native layers; portrait artwork is sampled from the supplied references. `M_ProfileGlass` is a UI-domain material driven by the same preview clock as Slate animation. Its editable authoring source is `Scripts/ProfileGlass.hlsl`; `Scripts/CreateInterfaceMaterial.py` regenerates the Unreal material asset during a full build. UI glow and procedural particles are composed in the interface renderer, not through scene bloom or Niagara.

The Workbench application's visual chrome has its own drawing primitives in `WorkbenchSkin.h` and a separate `M_WorkbenchAtmosphere` UI-domain material. `Scripts/WorkbenchAtmosphere.hlsl` and `Scripts/CreateWorkbenchAtmosphere.py` regenerate it during a full build. The atmosphere uses application wall time, not Player Profile preview time; its visual treatment therefore does not change component presets, component motion or saved design data. The app deliberately uses simple vector icons; no generated icon or theme-image atlas is part of the runtime.

Application cues are original 48 kHz mono sounds generated by `Scripts/GenerateUISounds.py` into `AudioSource/` and imported as `/Game/UISounds` SoundWaves by `Scripts/ImportUISounds.py`. `SWorkbench` loads these assets once and routes click and shortcut actions through `CueForAction`; a slider plays one selection cue at drag start. They use non-spatial 2D playback and do not follow the Player Profile playback clock. The `Sound On/Off` control is an application preference in `Interface.ini`, independent of saved component and UI Kit resources.

`ProfileData` is the saved base fixture. `Presentation` contains visual and motion settings. `ProfileState` controls layout and interaction. `Playback` supplies presentation time. Synthetic rewards are timestamped preview events; the near-threshold fixture uses a separate preview XP value and never changes the saved profile. The progress part takes the resulting normalized value and can later be fed by a different data source. Component and global-resource editors remember their selected categories independently; returning to an already active scope does not reload or discard a draft.

Future game flow should pass domain events through a game-data service: photo capture/evaluation or other actions update the appropriate player and episode records, then UI components read a stable data snapshot and play reactions to those changes. Global player data, per-episode starting snapshots and episode-specific deltas should remain separate from visual definitions and their preset links. The current synthetic reward buttons only exercise the presentation side of that contract; this milestone does not create a production progression or episode schema.

## Persistence and current limits

`%LOCALAPPDATA%/KOALITIC/WorkbenchV03/Presentation.ini` stores shared base profile data plus each visual variant's composition, links and local values; `UIKit.ini` stores shared category resources; `Interface.ini` stores the sound toggle. Older presentation files without variants load as one Original design and are migrated on the next Save component. Preview rewards, pause time and unsaved drafts are not persisted. Legacy `Style` and `Motion` keys can still be read as a migration fallback; new saves use the registered local-value schema. The old `MotionPresets.ini` route is inactive and its file is not deleted.

The code is more modular than the first prototype, but it is **not yet a generic multi-component framework**: `KitParameters.h` and `Presentation` describe Player Profile, and `SWorkbench` still coordinates much of the screen and session. Before adding another component, define its data contract, reusable parts and category/binding needs. Do not clone the Player Profile or invent a universal schema prematurely.
