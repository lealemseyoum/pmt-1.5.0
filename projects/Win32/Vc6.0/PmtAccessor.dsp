# Microsoft Developer Studio Project File - Name="PmtAccessor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PmtAccessor - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PmtAccessor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PmtAccessor.mak" CFG="PmtAccessor - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PmtAccessor - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PmtAccessor - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PmtAccessor - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PmtAccessor - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PmtAccessor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtAccessorRelease"
# PROP Intermediate_Dir "PmtAccessorRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /I /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtAccessor.lib"

!ELSEIF  "$(CFG)" == "PmtAccessor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtAccessor___Win32_Debug"
# PROP BASE Intermediate_Dir "PmtAccessor___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtAccessorDebug"
# PROP Intermediate_Dir "PmtAccessorDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\Include" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtAccessord.lib"

!ELSEIF  "$(CFG)" == "PmtAccessor - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtAccessor___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "PmtAccessor___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtAccessorUnicodeDebug"
# PROP Intermediate_Dir "PmtAccessorUnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(KIESEXIFROOT)\inc" /I "$(JPEGROOT)\inc" /I "C:\Program Files\Microsoft SDK\Include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /Zm120 /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\PmtAccessord.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtAccessorud.lib"

!ELSEIF  "$(CFG)" == "PmtAccessor - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PmtAccessor___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "PmtAccessor___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PmtAccessorUnicodeRelease"
# PROP Intermediate_Dir "PmtAccessorUnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(KIESEXIFROOT)\inc" /I "$(JPEGROOT)\inc" /I "C:\Program Files\Microsoft SDK\Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\PmtAccessor.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\PmtAccessoru.lib"

!ENDIF 

# Begin Target

# Name "PmtAccessor - Win32 Release"
# Name "PmtAccessor - Win32 Debug"
# Name "PmtAccessor - Win32 Unicode Debug"
# Name "PmtAccessor - Win32 Unicode Release"
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
# End Target
# End Project
