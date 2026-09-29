# Project status and next decisions

## Implemented

- Standalone Unreal Engine 5.8 Windows application with a main menu, functional UI Workbench and reserved Episodes/Editor screens.
- One layered Player Profile with expanded/compact states, editable profile fields, multiple saved visual design variants and a reusable progress part. Compact omits the portrait and numeric XP label.
- Eight independent entrance and eight exit algorithms with editable displacement, energy, sparks and glitch; independently timed ambient UI material/geometry effects; and a shared presentation clock.
- Shared Theme, Motion FX (Entrance/Exit), Ambient FX, Reaction FX (Pulse) and Event FX (XP gain/level-up) resources. XP Bar geometry styling is local to the Player Profile design while its color can follow the Theme. The inspector has scrollable grouped controls and preserves unsaved drafts separately per preset.
- Primary Workbench navigation plus a closable Library sidebar containing the component card, design variants and layers. Resource presets and controls stay together in the right inspector while the live preview expands. Component editing separates Data & links from Design overrides, with layer-specific controls when a layer is selected. The duplicate Pieces/Components destinations and Layout Studies thumbnails are removed. Preview & Test groups state, one-shot motion, synthetic events and layout conditions. Categories are color-coded across navigation, inspector and binding rows. The build script packages to one stable local path and regenerates its launch shortcut.
- Synthetic XP events and level-crossing preview, separate from saved base data.
- Six original sci-fi application sound cues for navigation, selection, confirmation, entrance, exit and synthetic rewards; a persistent Sound On/Off control. They are separate from component sound authoring.
- A separate application visual skin inspired by the approved sci-fi interface concept: dark beveled panels, glass header bands, lit control wells, neon category rails, a framed preview and matching inspector controls. `WorkbenchSkin.h` contains app-only drawing primitives. `M_WorkbenchAtmosphere` adds subtle animated grid and light to the app background; its source is reproducible in `Scripts/`. The Player Profile renderer and saved resources are independent of this chrome. The brief experiment with generated icon/theme atlases was removed; the runtime keeps simple vector icons.
- A packaged Windows build and an opt-in verification harness. An earlier package passed 23 automated checks. The current visual-skin package was built and launched standalone; captured Workbench, resource-editor and main-menu screens were inspected, and two alignment/gradient defects were fixed. Subjective visual and auditory approval remains with the product owner.

## Deliberately deferred

No additional major component, functional episode/asset library, video editor, generic component factory, production progression dataset, video export, component sound authoring or freeform keyframe timeline has been started. The current parameters and persistence schema are specific to Player Profile even though store, binding, navigation and control-rendering responsibilities have been separated.

## Next product discussion

The immediate checkpoint is the owner's visual review of the refreshed application interface. The owner plans to provide the full list of future components, their behavior and shared data relationships before a second component is implemented. Do not treat historical mockup labels as implemented features or as new scope approval.

The original reference images and videos remain under `references/`. The Nightfall precedent and early prompts are archived only for historical context.
