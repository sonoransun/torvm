' Copyright (C) 2009  The Tor Project, Inc.
' See LICENSE file for rights and terms.
'
' Script to generate an XML description of all installed
' products and packages.
'   Ex.: cscript.exe //Nologo pkginfo.vbs > pkginfo.xml
'
Option Explicit
Const msiInstallStateBadConfig    = -6
Const msiInstallStateInvalidArg   = -2
Const msiInstallStateUnknown      = -1
Const msiInstallStateAdvertised   =  1
Const msiInstallStateAbsent       =  2
Const msiInstallStateDefault      =  5
'
' Connect to Windows Installer object and list package details
Dim installer
Set installer = Wscript.CreateObject("WindowsInstaller.Installer")
Dim product, products
Dim productState
Set products = installer.Products
For Each product In products
  WScript.Echo GenerateProductElement()
  WScript.Echo GeneratePackageElement()
  WScript.Echo "</Product>" & vbCrLf
Next
'
' All done.
Set products = Nothing
Set installer = Nothing
Wscript.Quit 0
'
' Helper functions
Function GenerateProductElement()
  Dim script
  script = "<Product Id=""" & product & """" & vbCrLf
  script = script & ProductProperty("InstalledProductName", "Name", 6) & vbCrLf
  script = script & ProductProperty("VersionString", "Version", 6) & vbCrLf
  script = script & ProductProperty("InstallDate", "InstalledOn", 6) & vbCrLf
  script = script & Space(6) & "State="""
  Select Case installer.ProductState(product)
    Case msiInstallStateAbsent:
      script = script & "LocalDifferentUser"
    Case msiInstallStateDefault:
      script = script & "LocalCurrentUser"
    Case msiInstallStateAdvertised:
      script = script & "Advertised"
    Case msiInstallStateBadConfig:
      script = script & "Corrupt"
  End Select
  script = script & """" & vbCrLf
  script = script & ProductProperty("InstallSource", "InstalledFrom", 6) & vbCrLf
  script = script & ProductProperty("LocalPackage", "LocalPackage", 6)
  GenerateProductElement = script & ">"
End Function

Function GeneratePackageElement()
  Dim script
  script = "   <Package " & ProductProperty("PackageCode", "Id", 0) & " />"
  GeneratePackageElement = script
End Function
 
Function ProductProperty(attribute, name, indent)
  ProductProperty = Space(indent) & name & "=""" & installer.ProductInfo(product, attribute) & """"
End Function
