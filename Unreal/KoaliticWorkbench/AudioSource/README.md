# Original application cues

The six `UI_*.wav` files are synthesized specifically for this project by `Scripts/GenerateUISounds.py`. They use no recorded, sampled or third-party audio. The deterministic generator is the editable source; the WAVs are intermediate sources; `/Game/UISounds/UI_*` are Unreal SoundWave imports used by the packaged application.

| Cue | Application use |
| --- | --- |
| Navigate | Scope, inspector, category, layout and page changes. |
| Select | Toggle, field, layer, preset and slider-start interaction. |
| Confirm | Save, apply and reload actions. |
| Enter | One-shot component entrance preview. |
| Exit | One-shot component exit preview. |
| Reward | Synthetic XP and pulse preview. |

These are Workbench interaction sounds. They are not the audio track of a Player Profile component preset. Rebuilding the project regenerates WAVs and imports them using the Unreal Editor asset pipeline.
