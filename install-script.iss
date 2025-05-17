; Sample Inno Setup Script for C++ Application

[Setup]
AppName=AlasGameEngine
AppVersion=1.0
DefaultDirName={localappdata}\AlasGameEngine
DefaultGroupName=AlasGameEngine
OutputDir=Output
OutputBaseFilename=AlasGameEngineInstaller
Compression=lzma
SolidCompression=yes

[Files]
Source: "bin\Release\game.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\alas_entry_point.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\imgui.ini"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\SDL3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\Release\Assets\*"; DestDir: "{app}\Assets"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "msvc\VC_redist.x86.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall
Source: "msvc\VC_redist.x64.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall

[Icons]
Name: "{group}\AlasGameEngine"; Filename: "{app}\alas_entry_point.exe"; WorkingDir: "{app}"; Comment: "Launch AlasGameEngine"
Name: "{userdesktop}\AlasGameEngine"; Filename: "{app}\alas_entry_point.exe"; Comment: "Launch AlasGameEngine"; Tasks: createDesktopShortcut

[Tasks]
Name: "createDesktopShortcut"; Description: "Create a desktop shortcut"; GroupDescription: "Additional Tasks"; Flags: unchecked

[UninstallDelete]
Type: files; Name: "{app}\game.exe"
Type: files; Name: "{app}\alas_entry_point.exe"
Type: files; Name: "{app}\SDL.dll"
Type: files; Name: "{app}"
Type: files; Name: "{app}\Assets\*"


[Run]
; Run the 32-bit redistributable silently if the system is 32-bit or if the application is 32-bit on a 64-bit system
Check: not Is64BitInstallMode; Filename: "{tmp}\VC_redist.x86.exe"; Parameters: "/install /quiet /norestart"; Flags: waituntilterminated

; Run the 64-bit redistributable silently if the system is 64-bit
Check: Is64BitInstallMode; Filename: "{tmp}\VC_redist.x64.exe"; Parameters: "/install /quiet /norestart"; Flags: waituntilterminated