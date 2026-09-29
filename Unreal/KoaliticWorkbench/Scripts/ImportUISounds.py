"""Import AudioSource WAV files through Unreal Editor's supported asset pipeline."""
from pathlib import Path
import unreal

source = Path(__file__).resolve().parents[1] / "AudioSource"
destination = "/Game/UISounds"
unreal.EditorAssetLibrary.make_directory(destination)
tasks = []
for name in ("Navigate", "Select", "Confirm", "Enter", "Exit", "Reward"):
    wav = source / f"UI_{name}.wav"
    if not wav.is_file():
        raise RuntimeError(f"Missing source audio: {wav}")
    task = unreal.AssetImportTask()
    task.set_editor_property("filename", str(wav))
    task.set_editor_property("destination_path", destination)
    task.set_editor_property("destination_name", f"UI_{name}")
    task.set_editor_property("replace_existing", True)
    task.set_editor_property("automated", True)
    task.set_editor_property("save", True)
    tasks.append(task)
unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
for name in ("Navigate", "Select", "Confirm", "Enter", "Exit", "Reward"):
    path = f"{destination}/UI_{name}"
    asset = unreal.EditorAssetLibrary.load_asset(path)
    if not asset:
        raise RuntimeError(f"Could not import {path}")
    unreal.EditorAssetLibrary.save_loaded_asset(asset)
    unreal.log(f"UI_SOUND_IMPORTED {path} {asset.get_class().get_name()}")
