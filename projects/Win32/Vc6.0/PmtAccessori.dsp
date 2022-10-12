# Microsoft Developer Studio Project File - Name="PmtAccessori" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PmtAccessori - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PmtAccessori.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PmtAccessori.mak" CFG="PmtAccessori - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PmtAccessori - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PmtAccessori - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PmtAccessori - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PmtAccessori - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PmtAccessori - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtAccessorReleasei"
# PROP Intermediate_Dir "PmtAccessorReleasei"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PMTCORE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMT_DLL" /D "PMTACCR_IMPL_BUILD_DLL" /D "EXIF_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ole32.lib Eki.lib PmtCorei.lib openexifi.lib ektiffi.lib /nologo /dll /machine:I386 /out:"..\..\..\lib\PmtAccessori.dll" /implib:"..\..\..\lib\PmtAccessori.lib" /libpath:"$(EXIFROOT)\lib\release" /libpath:"..\..\..\lib" /libpath:"$(TIFFROOT)\lib" /libpath:"$(EXIFROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtAccessorDebugi"
# PROP Intermediate_Dir "PmtAccessorDebugi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PMTCORE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMT_DLL" /D "PMTACCR_IMPL_BUILD_DLL" /D "EXIF_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ole32.lib Ekid.lib PmtCoreid.lib openexifid.lib ektiffid.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\lib\PmtAccessorid.dll" /implib:"..\..\..\lib\PmtAccessorid.lib" /pdbtype:sept /libpath:"$(EXIFROOT)\lib\debug" /libpath:"..\..\..\lib" /libpath:"$(TIFFROOT)\lib" /libpath:"$(EXIFROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PmtAccessori___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "PmtAccessori___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtAccessorReleaseiu"
# PROP Intermediate_Dir "PmtAccessorReleaseiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(KIESEXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMT_DLL" /D "PMTACCR_IMPL_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMT_DLL" /D "PMTACCR_IMPL_BUILD_DLL" /D "EXIF_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Eki.lib PmtCorei.lib ekexifio140.lib ektiffi.lib /nologo /dll /machine:I386 /out:"..\..\..\lib\PmtAccessori.dll" /implib:"..\..\..\lib\PmtAccessori.lib" /libpath:"..\..\..\lib" /libpath:"$(EXIFROOT)\lib\release" /libpath:"$(TIFFROOT)\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib Ekiu.lib PmtCoreiu.lib openexifi.lib ektiffi.lib /nologo /dll /machine:I386 /out:"..\..\..\lib\PmtAccessoriu.dll" /implib:"..\..\..\lib\PmtAccessoriu.lib" /libpath:"$(EXIFROOT)\lib\release" /libpath:"..\..\..\lib" /libpath:"$(TIFFROOT)\lib" /libpath:"$(EXIFROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtAccessori___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "PmtAccessori___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtAccessorDebugiu"
# PROP Intermediate_Dir "PmtAccessorDebugiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(KIESEXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMT_DLL" /D "PMTACCR_IMPL_BUILD_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_DLL" /D "_USRDLL" /D "EK_DLL" /D "PMT_DLL" /D "PMTACCR_IMPL_BUILD_DLL" /D "EXIF_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Ekid.lib PmtCoreid.lib ekexifio140d.lib ektiffid.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\lib\PmtAccessorid.dll" /implib:"..\..\..\lib\PmtAccessorid.lib" /pdbtype:sept /libpath:"..\..\..\lib" /libpath:"$(EXIFROOT)\lib\debug" /libpath:"$(TIFFROOT)\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ole32.lib Ekiud.lib PmtCoreiud.lib openexifid.lib ektiffid.lib /nologo /dll /map /debug /machine:I386 /out:"..\..\..\lib\PmtAccessoriud.dll" /implib:"..\..\..\lib\PmtAccessoriud.lib" /pdbtype:sept /libpath:"$(EXIFROOT)\lib\debug" /libpath:"..\..\..\lib" /libpath:"$(TIFFROOT)\lib" /libpath:"$(EXIFROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PmtAccessori - Win32 Release"
# Name "PmtAccessori - Win32 Debug"
# Name "PmtAccessori - Win32 Unicode Release"
# Name "PmtAccessori - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtAllAccessors.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtDIG35Accessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtExifAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtExifImageAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtImageRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtJ2KAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtPngAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtTiffAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtTiffImageAccessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtTranslationTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtTranslator\PmtTranslator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtXmlAccessor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtAccrImplDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtAllAccessors.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtDIG35Accessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtExifAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtExifImageAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtImageAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtImageRect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtJ2KAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtPngAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtTiffAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtTiffImageAccessor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtTranslationTable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtTranslator\PmtTranslationTbl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtTranslator\PmtTranslator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\PmtAccessor\PmtXmlAccessor.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lib\Ekid.lib

!IF  "$(CFG)" == "PmtAccessori - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Debug"

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\PmtCoreid.lib

!IF  "$(CFG)" == "PmtAccessori - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Debug"

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\lib\Eki.lib

!IF  "$(CFG)" == "PmtAccessori - Win32 Release"

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\lib\PmtCorei.lib

!IF  "$(CFG)" == "PmtAccessori - Win32 Release"

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Release"

!ELSEIF  "$(CFG)" == "PmtAccessori - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
