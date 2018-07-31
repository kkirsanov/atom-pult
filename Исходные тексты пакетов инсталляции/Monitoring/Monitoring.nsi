; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "RobSim Monitoring"

; The file to write
OutFile "robsim_sensorika_Monitoring.exe"

; The default installation directory
InstallDir c:\Sensorika\Monitoring

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

  File screenClient.exe
  File config.ini
  File vcredist_x86_2008.exe
  ExecWait '"$INSTDIR\vcredist_x86_2008.exe" /Q'
  delete $INSTDIR\vcredist_x86_2008.exe


  SetShellVarContext current
  CreateShortCut "$DESKTOP\Monitoring.lnk" "$INSTDIR\screenClient.exe"

  MessageBox MB_OK "Система установленна в папку $INSTDIR и на рабочем столе созданна ссылка на неё"

SectionEnd ; end the section
