; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "RobSim Server"

; The file to write
OutFile "robsim_sensorika_server.exe"

; The default installation directory
InstallDir c:\Sensorika\server

;--------------------------------

; Pages

;Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there  

  File Server.exe
  File config.ini
  File vcredist_x86_2008.exe
  ExecWait '"$INSTDIR\vcredist_x86_2008.exe" /Q'
  delete $INSTDIR\vcredist_x86_2008.exe


  SetShellVarContext All
  CreateShortCut "$SMSTARTUP\Server.lnk" "$INSTDIR\Server.exe"

  MessageBox MB_OK "Система установленна в папку c:\Sensorika\server"


  MessageBox MB_YESNO|MB_ICONQUESTION "Запустить сейчас?" IDNO +2
    Exec '$INSTDIR/Server.exe'

SectionEnd ; end the section
