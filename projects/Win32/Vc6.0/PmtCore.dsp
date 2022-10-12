# Microsoft Developer Studio Project File - Name="PmtCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PmtCore - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PmtCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PmtCore.mak" CFG="PmtCore - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PmtCore - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PmtCore - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PmtCore - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PmtCore - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PmtCore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtCoreRelease"
# PROP Intermediate_Dir "PmtCoreRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtCore.lib"

!ELSEIF  "$(CFG)" == "PmtCore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtCoreDebug"
# PROP Intermediate_Dir "PmtCoreDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtCored.lib"

!ELSEIF  "$(CFG)" == "PmtCore - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtCore___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "PmtCore___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtCoreUnicodeDebug"
# PROP Intermediate_Dir "PmtCoreUnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_UNICODE" /D "UNICODE" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\PmtCored.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtCoreud.lib"

!ELSEIF  "$(CFG)" == "PmtCore - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PmtCore___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "PmtCore___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtCoreUnicodeRelease"
# PROP Intermediate_Dir "PmtCoreUnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_UNICODE" /D "UNICODE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\PmtCore.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtCoreu.lib"

!ENDIF 

# Begin Target

# Name "PmtCore - Win32 Release"
# Name "PmtCore - Win32 Debug"
# Name "PmtCore - Win32 Unicode Debug"
# Name "PmtCore - Win32 Unicode Release"
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
# End Target
# End Project
