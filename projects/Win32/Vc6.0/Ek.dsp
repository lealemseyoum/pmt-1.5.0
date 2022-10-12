# Microsoft Developer Studio Project File - Name="ek" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ek - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ek.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ek.mak" CFG="ek - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ek - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ek - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ek - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ek - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ek - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EkRelease"
# PROP Intermediate_Dir "EkRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /O2 /Ob2 /I "..\..\..\src\PmtCore" /I "..\..\..\src\Ek" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\Ek.lib"

!ELSEIF  "$(CFG)" == "ek - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EkDebug"
# PROP Intermediate_Dir "EkDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "LIB" /YX /FD /c
# SUBTRACT CPP /X /Fr
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\Ekd.lib"

!ELSEIF  "$(CFG)" == "ek - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ek___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ek___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EkUnicodeDebug"
# PROP Intermediate_Dir "EkUnicodeDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "..\..\..\src\Ek" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "LIB" /YX /FD /c
# SUBTRACT BASE CPP /X /Fr
# ADD CPP /nologo /G5 /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "_DEBUG" /D "_MBCS" /D "_AFXDLL" /D "LIB" /D "WIN32" /D "_UNICODE" /D "UNICODE" /YX /FD /c
# SUBTRACT CPP /X /Fr
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\Ekd.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\Ekud.lib"

!ELSEIF  "$(CFG)" == "ek - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ek___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "ek___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EkUnicodeRelease"
# PROP Intermediate_Dir "EkUnicodeRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GR /GX /O2 /Ob2 /I "..\..\..\src\Ek" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# SUBTRACT BASE CPP /X
# ADD CPP /nologo /G5 /MD /W3 /GR /GX /O2 /Ob2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_UNICODE" /D "UNICODE" /YX /FD /c
# SUBTRACT CPP /X
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\lib\Ek.lib"
# ADD LIB32 /nologo /out:"..\..\..\lib\Eku.lib"

!ENDIF 

# Begin Target

# Name "ek - Win32 Release"
# Name "ek - Win32 Debug"
# Name "ek - Win32 Unicode Debug"
# Name "ek - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\..\src\Ek\EkCompiler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkNullMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkPrecompiled.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkRefCount.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkStrings.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\ptstring.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\..\src\Ek\EkBool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkCompiler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkCompSmartPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkError.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkFactoryT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkGPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkGuard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkLocking.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkMapT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkNullMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkPrecompiled.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkRational.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkRefCount.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkSmartPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkStrings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkTemplate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkThread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\ptstring.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Build Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ek.mak
# End Source File
# End Group
# End Target
# End Project
