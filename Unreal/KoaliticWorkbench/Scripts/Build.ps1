param([string]$EngineRoot='E:/UnrealEngine/UE_5.8')
$ErrorActionPreference='Stop'
$ProjectRoot=Split-Path -Parent $PSScriptRoot
$WorkspaceRoot=Split-Path -Parent (Split-Path -Parent $ProjectRoot)
$ProjectFile=Join-Path $ProjectRoot 'KoaliticWorkbench.uproject'
& "$EngineRoot/Engine/Build/BatchFiles/Build.bat" KoaliticWorkbenchEditor Win64 Development "-Project=$ProjectFile" -WaitMutex
if($LASTEXITCODE -ne 0){throw 'Editor target build failed'}
& "$EngineRoot/Engine/Binaries/Win64/UnrealEditor-Cmd.exe" $ProjectFile -run=pythonscript "-script=$PSScriptRoot/CreateInterfaceMaterial.py" -unattended -nullrhi -nosplash
if($LASTEXITCODE -ne 0){throw 'UI material authoring failed'}
& "$EngineRoot/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun "-project=$ProjectFile" -noP4 -platform=Win64 -clientconfig=Development -build -cook -stage -pak -iostore -archive "-archivedirectory=$WorkspaceRoot/Builds/WorkbenchV03" -map=/Engine/Maps/Entry -utf8output
if($LASTEXITCODE -ne 0){throw 'Packaging failed'}
