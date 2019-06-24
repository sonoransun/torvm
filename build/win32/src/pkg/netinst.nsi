!include "MUI.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"
  
!define VERSION "0.0.3.0"
!define INSTALLER "TorVMNetInstaller.exe"
!define WEBSITE "https://www.torproject.org/"
!define LICENSE "LICENSE"
 
SetCompressor /SOLID BZIP2
RequestExecutionLevel admin
OutFile ${INSTALLER}
InstallDir "$PROGRAMFILES\TorInstPkgs"
SetOverWrite on
Name "Tor VM Network Installer"
Caption "Tor VM Network Installer"
BrandingText "Tor VM Network Installer"
CRCCheck on
XPStyle on
ShowInstDetails hide
VIProductVersion "${VERSION}"
VIAddVersionKey "ProductName" "Tor VM"
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
	File "thandy.msi"
FunctionEnd

Function RunInstallers
	ExecWait 'msiexec /i "$INSTDIR\thandy.msi" NOSC=1 /qn'
	ExecWait 'msiexec /i "$INSTDIR\license.msi" NOSC=1 /qn'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --force-check "--repo=$LOCALAPPDATA\Thandy\TorVM Updates" /bundleinfo/torvm/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --force-check "--repo=$LOCALAPPDATA\Thandy\Polipo Updates" /bundleinfo/polipo/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --force-check "--repo=$LOCALAPPDATA\Thandy\TorButton Updates" /bundleinfo/torbutton/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --force-check "--repo=$LOCALAPPDATA\Thandy\Vidalia Updates" /bundleinfo/vidalia/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --install "--repo=$LOCALAPPDATA\Thandy\TorVM Updates" /bundleinfo/torvm/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --install "--repo=$LOCALAPPDATA\Thandy\Polipo Updates" /bundleinfo/polipo/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --install "--repo=$LOCALAPPDATA\Thandy\TorButton Updates" /bundleinfo/torbutton/win32/'
	ExecWait '"$LOCALAPPDATA\Programs\Thandy\thandy.exe" update --install "--repo=$LOCALAPPDATA\Thandy\Vidalia Updates" /bundleinfo/vidalia/win32/'
        SetOutPath $DESKTOP
        File "Uninstall_Tor.bat"
FunctionEnd

Function LaunchTorVM
	SetOutPath "$PROGRAMFILES\Tor VM"
	Exec 'torvm.exe --bundle'
FunctionEnd
