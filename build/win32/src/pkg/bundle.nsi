!include "MUI.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"
  
!define VERSION "0.0.3.0"
!define INSTALLER "TorVMBundle.exe"
!define WEBSITE "https://www.torproject.org/"
!define LICENSE "LICENSE"
 
SetCompressor /SOLID BZIP2
RequestExecutionLevel admin
OutFile ${INSTALLER}
InstallDir "$PROGRAMFILES\TorInstPkgs"
SetOverWrite on
Name "Tor VM ${VERSION} Bundle"
Caption "Tor VM ${VERSION} Bundle Setup"
BrandingText "Tor VM Bundle Installer"
CRCCheck on
XPStyle on
ShowInstDetails hide
VIProductVersion "${VERSION}"
VIAddVersionKey "ProductName" "Tor VM Bundle"
VIAddVersionKey "Comments" "${WEBSITE}"
VIAddVersionKey "LegalTrademarks" "Three line BSD"
VIAddVersionKey "LegalCopyright" "©2004-2009, Roger Dingledine, Nick Mathewson, The Tor Project, Inc."
VIAddVersionKey "FileDescription" "Tor is an implementation of Onion Routing. You can read more at ${WEBSITE}"
VIAddVersionKey "FileVersion" "${VERSION}"

!define MUI_ICON "torinst32.ico"
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\win.bmp"
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "TorVM" TorVM
	SectionIn RO
	SetOutPath $INSTDIR
	Call ExtractPackages
        Call RunInstallers
	Call LaunchTorVM
SectionEnd

Function ExtractPackages
	File "license.msi"
	File "torvm.msi"
	File "geoip.msi"
	File "torbutton.msi"
	File "thandy.msi"
	File "polipo.msi"
	File "vidalia.msi"
FunctionEnd

Function RunInstallers
	ExecWait 'msiexec /i "$INSTDIR\torvm.msi" BUNDLE=1 /qn'
	ExecWait 'msiexec /i "$INSTDIR\geoip.msi" /qn'
	ExecWait 'msiexec /i "$INSTDIR\vidalia.msi" NOSC=1 /qn'
	ExecWait 'msiexec /i "$INSTDIR\thandy.msi" NOSC=1 /qn'
	ExecWait 'msiexec /i "$INSTDIR\polipo.msi" NOSC=1 /qn'
	ExecWait 'msiexec /i "$INSTDIR\torbutton.msi" NOSC=1 /qn'
	ExecWait 'msiexec /i "$INSTDIR\license.msi" NOSC=1 /qn'
        SetOutPath $DESKTOP
        File "Uninstall_Tor.bat"
FunctionEnd

Function LaunchTorVM
	SetOutPath "$PROGRAMFILES\Tor VM"
	Exec 'torvm.exe --bundle'
FunctionEnd

