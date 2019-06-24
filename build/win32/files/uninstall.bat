@ECHO OFF
ECHO Removing installed https://www.torproject.org software.
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\TorVM Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\TorVM Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "TorVM Updates"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\GeoIP Data Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\GeoIP Data Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "GeoIP Data Updates"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\Tor Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\Tor Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "Tor Updates"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\Polipo Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\Polipo Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "Polipo Updates"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\TorButton Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\TorButton Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "TorButton Updates"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\Vidalia Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\Vidalia Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "Vidalia Updates"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Thandy\Vidalia Marble Updates" (
  cd "%USERPROFILE%\Local Settings\Application Data\Thandy\Vidalia Marble Updates"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q "Vidalia Marble Updates"
)
IF EXIST %PROGRAMFILES%\TorInstPkgs (
  cd %PROGRAMFILES%\TorInstPkgs
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q TorInstPkgs
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\TorInstPkgs" (
  cd "%USERPROFILE%\Local Settings\Application Data\TorInstPkgs"
  for %%f in (*.msi) do ECHO Removing %%f
  for %%f in (*.msi) do msiexec /x %%f /qn
  cd ..
  rmdir /S /Q TorInstPkgs
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Programs\Polipo" (
  rmdir /S /Q "%USERPROFILE%\Local Settings\Application Data\Programs\Polipo"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Programs\Thandy" (
  rmdir /S /Q "%USERPROFILE%\Local Settings\Application Data\Programs\Thandy"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Programs\Vidalia" (
  rmdir /S /Q "%USERPROFILE%\Local Settings\Application Data\Programs\Vidalia"
)
IF EXIST "%USERPROFILE%\Local Settings\Application Data\Programs\Tor License" (
  rmdir /S /Q "%USERPROFILE%\Local Settings\Application Data\Programs\Tor License"
)
IF EXIST "%SYSTEMDRIVE%\Documents and Settings\Tor" (
  net user Tor /DELETE
  rmdir /S /Q "%SYSTEMDRIVE%\Documents and Settings\Tor"
)
IF EXIST "%ALLUSERSPROFILE%\Application Data\Microsoft\User Account Pictures\Tor.bmp" (
  rmdir /S /Q "%ALLUSERSPROFILE%\Application Data\Microsoft\User Account Pictures\Tor.bmp"
)
IF EXIST "%PROGRAMFILES%\Tor VM" (
  rmdir /S /Q "%PROGRAMFILES%\Tor VM"
)
IF EXIST "%USERPROFILE%\Desktop\Uninstall_Tor.bat" (
  del /F "%USERPROFILE%\Desktop\Uninstall_Tor.bat"
)
