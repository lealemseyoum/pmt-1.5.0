# Microsoft Developer Studio Project File - Name="PmtCorei" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PmtCorei - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PmtCorei.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PmtCorei.mak" CFG="PmtCorei - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PmtCorei - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PmtCorei - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PmtCorei - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PmtCorei - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PmtCorei - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtCoreReleasei"
# PROP Intermediate_Dir "PmtCoreReleasei"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PMTCORE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMTCORE_BUILD_DLL" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ole32.lib Eki.lib /nologo /dll /machine:I386 /out:"..\..\..\lib\PmtCorei.dll" /implib:"..\..\..\lib\PmtCorei.lib" /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtCorei - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtCoreDebugi"
# PROP Intermediate_Dir "PmtCoreDebugi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PMTCORE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMTCORE_BUILD_DLL" /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ole32.lib Ekid.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\lib\PmtCoreid.dll" /implib:"..\..\..\lib\PmtCoreid.lib" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtCorei - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PmtCorei___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "PmtCorei___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtCoreReleaseiu"
# PROP Intermediate_Dir "PmtCoreReleaseiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMTCORE_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Ek" /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMTCORE_BUILD_DLL" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib Ekiu.lib /nologo /dll /machine:I386 /out:"..\..\..\lib\PmtCorei.dll" /implib:"..\..\..\lib\PmtCorei.lib" /libpath:"..\..\..\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib Ekiu.lib /nologo /dll /machine:I386 /out:"..\..\..\lib\PmtCoreiu.dll" /implib:"..\..\..\lib\PmtCoreiu.lib" /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtCorei - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtCorei___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "PmtCorei___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtCoreDebugiu"
# PROP Intermediate_Dir "PmtCoreDebugiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMTCORE_BUILD_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMTCORE_BUILD_DLL" /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib Ekiud.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\lib\PmtCoreid.dll" /implib:"..\..\..\lib\PmtCoreid.lib" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib Ekiud.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\lib\PmtCoreiud.dll" /implib:"..\..\..\lib\PmtCoreiud.lib" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PmtCorei - Win32 Release"
# Name "PmtCorei - Win32 Debug"
# Name "PmtCorei - Win32 Unicode Release"
# Name "PmtCorei - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtAliases.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtAliasesParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtAttributeT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtCompositeMetadata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtDefinitionInterpreter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtFacet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtInstanceInterpreter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtKeyParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtMetadata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtMetadataT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtMisc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtModelGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtOccurrence.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSchemaInfoParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSchemaSAXHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSchemaSAXParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtXMLInstanceParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtAliases.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtAliasesParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtAttribute.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtAttributeT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtCompositeMetadata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtCoreCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtCoreDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtDefaultPreparsedSchema.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtDefinitionInterpreter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtDefinitionTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtDomString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtDoubleMdVisitorT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtElement.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtError.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtFacet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtFacetT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtInstanceInterpreter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtIoFunctions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtKeyParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtMdVisitorImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtMetadata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtMetadataT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtMetadataVisitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtMisc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtModelGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtOccurrence.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtOrder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtRootMetadata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSAX2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSchemaInfoParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSchemaSAXHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtSchemaSAXParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtCore\PmtSingleMdVisitorT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtUtils\PmtVersionNumber.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtInterpreter\PmtXMLInstanceParser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
