# Workbench workflow

The application currently has one implemented library component: **Player Profile**. `01 Components` edits its data, layers, states and local values. `02 UI Kit` creates and edits category resources shared with that component. UI Kit categories are Entrance, Exit, Ambient FX, Reaction FX, Appearance and Progress Bar; Entrance and Exit are independent.

## Preview and test

Select **Expanded** or **Compact** to hold the component in that layout. Ambient motion continues while the preview plays. **Replay enter**, **Preview exit** and **Pulse** are momentary previews; they return to the selected layout. **Pause preview** freezes the presentation clock and shader effects. **Reset test** restarts the preview and clears synthetic rewards. Hover, pinning and disabled appearance can be inspected in Components.

The test buttons add synthetic XP events. **Near threshold** prepares a level crossing; **Clear rewards** removes preview events. These events are not production progression and are not saved with the component. **Edit XP bar / Solo** isolates the progress part; the Progress Bar category changes its shape and effects independently of profile motion.

## Shared resource versus local override

1. In UI Kit, choose a category and select a named resource to audition it. Adjust its controls; a numeric value can also be typed directly. Drafts survive category navigation during the current session.
2. **Save preset** writes the named resource. **Save as...** prepares a separate draft name; edit the name and save it. If the saved name matches the component's current link, its shared values update immediately except where local overrides exist.
3. **Apply** links the saved resource to the component. This does not save the component assignment to disk.
4. In Components, moving or typing a parameter creates a local override. **Reset** beside it restores inheritance; **Reset this category** clears that category's overrides.
5. **Save component** persists the base profile, resource links and local overrides. It does not save unsaved UI Kit drafts or synthetic rewards.

Undo/Redo covers in-memory parameter edits and resource operations represented by the edit snapshot. It does not undo writes already made to `UIKit.ini`, base data-field edits or synthetic XP events. **Reload** restores saved component state. A visual Appearance variant can change without altering Entrance or Exit.

Current ranges are visible alongside controls. Some signed speeds reverse movement below zero; nonnegative quantities such as glow and density can exceed the normal intensity but cannot be negative. The current presentation offers five entrance and five exit algorithms, two authored frame variants, and several ambient modules. It is not an arbitrary motion-keyframe editor or freeform geometry editor.
