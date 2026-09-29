param([string]$EngineRoot='E:/UnrealEngine/UE_5.8')
$ErrorActionPreference='Stop'
$ProjectRoot=Split-Path -Parent $PSScriptRoot
$WorkspaceRoot=Split-Path -Parent (Split-Path -Parent $ProjectRoot)
$ProjectFile=Join-Path $ProjectRoot 'KoaliticWorkbench.uproject'
$PackageRoot=Join-Path $WorkspaceRoot 'Builds/Workbench'
$Executable=Join-Path $PackageRoot 'Windows/KoaliticWorkbench.exe'
$ShortcutPath=Join-Path $WorkspaceRoot 'Launch KOALITIC Workbench.lnk'
& python "$PSScriptRoot/GenerateUISounds.py"
if($LASTEXITCODE -ne 0){throw 'UI sound generation failed'}
& "$EngineRoot/Engine/Build/BatchFiles/Build.bat" KoaliticWorkbenchEditor Win64 Development "-Project=$ProjectFile" -WaitMutex
if($LASTEXITCODE -ne 0){throw 'Editor target build failed'}
& "$EngineRoot/Engine/Binaries/Win64/UnrealEditor-Cmd.exe" $ProjectFile -run=pythonscript "-script=$PSScriptRoot/PrepareAssets.py" -unattended -nullrhi -nosplash
if($LASTEXITCODE -ne 0){throw 'UI asset authoring failed'}
& "$EngineRoot/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun "-project=$ProjectFile" -noP4 -platform=Win64 -clientconfig=Development -build -cook -stage -pak -iostore -archive "-archivedirectory=$PackageRoot" -map=/Engine/Maps/Entry -utf8output
if($LASTEXITCODE -ne 0){throw 'Packaging failed'}
if(!(Test-Path -LiteralPath $Executable)){throw "Packaged executable missing: $Executable"}
$Shell=New-Object -ComObject WScript.Shell
$Shortcut=$Shell.CreateShortcut($ShortcutPath)
$Shortcut.TargetPath=$Executable
$Shortcut.WorkingDirectory=Split-Path -Parent $Executable
$Shortcut.Description='KOALITIC UI Workbench'
$Shortcut.IconLocation="$Executable,0"
$Shortcut.Save()
Write-Output "Packaged app: $Executable"
Write-Output "Launch shortcut: $ShortcutPath"
