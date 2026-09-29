"""Author editable Unreal material and import original UI cue WAVs in one editor session."""
from pathlib import Path
import runpy

scripts = Path(__file__).resolve().parent
runpy.run_path(str(scripts / "CreateInterfaceMaterial.py"))
runpy.run_path(str(scripts / "CreateWorkbenchAtmosphere.py"))
runpy.run_path(str(scripts / "ImportUISounds.py"))
